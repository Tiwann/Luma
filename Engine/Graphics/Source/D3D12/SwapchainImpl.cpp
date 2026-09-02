#include "Luma/D3D12/SwapchainImpl.h"
#include "Luma/D3D12/Conversions.h"
#include "Luma/D3D12/RenderDeviceImpl.h"
#include "Luma/Runtime/DesktopWindow.h"
#include <dxgi1_6.h>
#include <directx/d3d12.h>
#include <rgfw/rgfw.h>

namespace Luma::D3D12
{
    bool FSwapchainImpl::initialize(const FSwapchainDesc& desc)
    {
        if (!desc.device) return false;

        FRenderDeviceImpl* device = static_cast<FRenderDeviceImpl*>(desc.device);
        FDesktopWindow* window = device->getWindow();
        IDXGIFactory7* factory = device->getFactory();

        DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
        swapchainDesc.Flags = 0;
        swapchainDesc.BufferCount = (uint32_t)desc.buffering;
        swapchainDesc.Width = desc.width;
        swapchainDesc.Height = desc.height;
        swapchainDesc.Format = convert<DXGI_FORMAT>(desc.format);
        swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapchainDesc.SwapEffect = convert<DXGI_SWAP_EFFECT>(desc.presentMode);
        swapchainDesc.SampleDesc.Count = 1;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapchainFullscreenDesc = {};
        swapchainFullscreenDesc.RefreshRate = DXGI_RATIONAL(0, 0); // Using native display refresh rate
        swapchainFullscreenDesc.Windowed = false;
        swapchainFullscreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        swapchainFullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

        const HWND hwnd = static_cast<HWND>(RGFW_window_getHWND(window->getHandle()));
        IDXGISwapChain1* swapchain = nullptr;
        if (DX_FAILED(factory->CreateSwapChainForHwnd(device->getHandle(), hwnd, &swapchainDesc, nullptr, nullptr, &swapchain)))
            return false;

        if (m_Handle) m_Handle->Release();

        if (DX_FAILED(swapchain->QueryInterface(IID_PPV_ARGS(&m_Handle))))
            return false;


        ID3D12Device13* deviceHandle = device->getHandle();
        const uint32_t descriptorSize = deviceHandle->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        D3D12_DESCRIPTOR_HEAP_DESC renderTargetViewHeapDesc = {};
        renderTargetViewHeapDesc.NumDescriptors = (uint32_t)desc.buffering;
        renderTargetViewHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

        if (DX_FAILED(deviceHandle->CreateDescriptorHeap(&renderTargetViewHeapDesc, IID_PPV_ARGS(&m_DescriptorHeap))))
            return false;

        for (size_t imageIndex = 0; imageIndex < (size_t)desc.buffering; imageIndex++)
        {
            ID3D12Resource* image = nullptr;
            if (DX_FAILED(m_Handle->GetBuffer(imageIndex, IID_PPV_ARGS(&image))))
                return false;

            if (m_Images[imageIndex]) m_Images[imageIndex]->Release();
            if (DX_FAILED(image->QueryInterface(IID_PPV_ARGS(&m_Images[imageIndex]))))
                return false;

            D3D12_RENDER_TARGET_VIEW_DESC rtvDesc;
            rtvDesc.Format = convert<DXGI_FORMAT>(desc.format);
            rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
            rtvDesc.Texture2D.MipSlice = 0;
            rtvDesc.Texture2D.PlaneSlice = 0;

            m_ImageViews[imageIndex] = m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart().ptr + imageIndex * descriptorSize;
            deviceHandle->CreateRenderTargetView(m_Images[imageIndex], nullptr, {m_ImageViews[imageIndex]});
        }

        m_Device = desc.device;
        m_Buffering = desc.buffering;
        m_Width = desc.width;
        m_ImageHeight = desc.height;
        m_ImageFormat = desc.format;
        m_ImagePresentMode = desc.presentMode;
        m_Valid = true;
        return true;
    }

    void FSwapchainImpl::destroy()
    {
        if (m_Handle) m_Handle->Release();

        for (size_t imageIndex = 0; imageIndex < m_Device->getFrameCount(); imageIndex++)
            m_Images[imageIndex]->Release();
    }

    bool FSwapchainImpl::resize(const uint32_t width, const uint32_t height)
    {
        if (!m_Device) return false;
        m_Device->waitIdle();

        const FRenderDeviceImpl* device = static_cast<FRenderDeviceImpl*>(m_Device);
        const uint32_t bufferCount = device->getFrameCount();
        const DXGI_FORMAT format = convert<DXGI_FORMAT>(m_ImageFormat);
        if (DX_FAILED(m_Handle->ResizeBuffers(bufferCount, width, height, format, 0)))
            return false;
        return true;
    }
}
