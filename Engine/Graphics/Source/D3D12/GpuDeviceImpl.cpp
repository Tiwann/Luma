#include "Luma/D3D12/GpuDeviceImpl.h"
#include "Luma/Runtime/DesktopWindow.h"
#include <directx/d3d12.h>
#include <dxgi1_6.h>

#include "Luma/D3D12/Buffer.h"

namespace Luma::D3D12
{
#if defined(LUMA_DEBUG) || defined(LUMA_DEV)
    static void DebugCallback(D3D12_MESSAGE_CATEGORY category, const D3D12_MESSAGE_SEVERITY severity,
                              D3D12_MESSAGE_ID id, const LPCSTR desc, void* pContext)
    {
        switch (severity)
        {
        case D3D12_MESSAGE_SEVERITY_ERROR:
        case D3D12_MESSAGE_SEVERITY_CORRUPTION:
            std::println(std::cerr, "Direct3D Error: {}", desc);
            return;
        case D3D12_MESSAGE_SEVERITY_WARNING:
            std::println(std::cout, "Direct3D Warning: {}", desc);
        default:
            break;
        }
    }
#endif
    
    EGpuDeviceType FGpuDeviceImpl::getDeviceType()
    {
        return EGpuDeviceType::D3D12;
    }

    bool FGpuDeviceImpl::initialize(const FGpuDeviceDesc& deviceDesc)
    {
        IDXGIFactory7* factory;
        if (DX_FAILED(CreateDXGIFactory2(0, IID_PPV_ARGS(&factory))))
            return false;

        uint32_t adapterIndex = 0;
        IDXGIAdapter4* adapter = nullptr;
        while (factory->EnumAdapters1(adapterIndex, (IDXGIAdapter1**)&adapter) != DXGI_ERROR_NOT_FOUND)
        {
            DXGI_ADAPTER_DESC1 description;
            if (DX_FAILED(adapter->GetDesc1(&description)))
                return false;

            if (description.DedicatedVideoMemory > 0)
            {
                m_Adapter = adapter;
                break;
            }
            ++adapterIndex;
        }

#if defined(LUMA_DEBUG) || defined(LUMA_DEV)
        if (DX_FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_Debug))))
            return false;
        m_Debug->EnableDebugLayer();
#endif
        if (DX_FAILED(D3D12CreateDevice(m_Adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_Handle))))
            return false;

#if defined(LUMA_DEBUG) || defined(LUMA_DEV)
        if (DX_FAILED(m_Handle->QueryInterface(IID_PPV_ARGS(&m_InfoQueue))))
            return false;

        (void)m_InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, false);
        (void)m_InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, false);

        /*D3D12_INFO_QUEUE_FILTER filter = {};
        (void)m_InfoQueue->PushStorageFilter(&filter);*/

        DWORD callbackCookie = 0;
        if (DX_FAILED(
            m_InfoQueue->RegisterMessageCallback(DebugCallback, D3D12_MESSAGE_CALLBACK_FLAG_NONE, nullptr, &
                callbackCookie)))
            return false;
#endif


        if (!createCommandSignatures())
            return false;

        m_Window = static_cast<FDesktopWindow*>(deviceDesc.window);

        const auto getPresentMode = [this](const bool vSync)
        {
            DXGI_SWAP_EFFECT result;

            if (vSync)
            {
                result = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
                BOOL mailboxSupported = false;
                (void)m_Factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &mailboxSupported,
                                                     sizeof(mailboxSupported));
                if (mailboxSupported)
                    result = DXGI_SWAP_EFFECT_FLIP_DISCARD;
            }
            else
            {
                result = DXGI_SWAP_EFFECT_FLIP_DISCARD;
            }

            switch (result)
            {
            case DXGI_SWAP_EFFECT_DISCARD: return EPresentMode::Immediate;
            case DXGI_SWAP_EFFECT_FLIP_DISCARD: return EPresentMode::Fifo;
            default: return EPresentMode::Unknown;
            }
        };


        m_RenderQueue.initialize(this, EQueueType::Render);
        m_ComputeQueue.initialize(this, EQueueType::Compute);
        m_CopyQueue.initialize(this, EQueueType::Copy);

        FSwapchainDesc swapchainDesc;
        swapchainDesc.device = this;
        swapchainDesc.format = EFormat::R8G8B8A8_UNORM;
        swapchainDesc.width = m_Window->getWidth();
        swapchainDesc.height = m_Window->getHeight();
        swapchainDesc.buffering = deviceDesc.buffering;
        swapchainDesc.presentMode = getPresentMode(deviceDesc.vSync);
        if (!m_Swapchain.initialize(swapchainDesc))
            return false;
    }

    void FGpuDeviceImpl::destroy()
    {
    }

    bool FGpuDeviceImpl::beginFrame()
    {
    }

    void FGpuDeviceImpl::endFrame()
    {
    }

    void FGpuDeviceImpl::present()
    {
    }

    void FGpuDeviceImpl::waitIdle()
    {
    }

    uint32_t FGpuDeviceImpl::getFrameCount() const
    {
    }

    uint32_t FGpuDeviceImpl::getCurrentFrameIndex()
    {
    }

    bool FGpuDeviceImpl::hasVSync()
    {
        return m_Swapchain.hasVSync();
    }

    ISwapchain* FGpuDeviceImpl::getSwapchain()
    {
        return &m_Swapchain;
    }

    IQueue* FGpuDeviceImpl::getRenderQueue()
    {
        return &m_RenderQueue;
    }

    IQueue* FGpuDeviceImpl::getComputeQueue()
    {
        return &m_ComputeQueue;
    }

    IQueue* FGpuDeviceImpl::getCopyQueue()
    {
        return &m_CopyQueue;
    }

    IBuffer* FGpuDeviceImpl::createBuffer(const FBufferDesc& bufferDesc)
    {
        FBufferImpl* buffer = new FBufferImpl();
        FBufferDesc desc{bufferDesc};
        desc.device = this;

        if (!buffer->initialize(desc))
        {
            delete buffer;
            return nullptr;
        }

        return buffer;
    }

    ITexture* FGpuDeviceImpl::createTexture(const FTextureDesc& textureDesc)
    {
    }

    ITextureView* FGpuDeviceImpl::createTextureView(const FTextureViewDesc& textureViewDesc)
    {
    }

    IShaderProgram* FGpuDeviceImpl::createShader(const FShaderDesc& shaderDesc)
    {
    }

    ICommandBuffer* FGpuDeviceImpl::createCommandBuffer(const FCommandBufferDesc& cmdBufferDesc)
    {
    }

    ICommandBuffer* FGpuDeviceImpl::getCommandBuffer()
    {
    }

    ISampler* FGpuDeviceImpl::createSampler(const FSamplerDesc& samplerDesc)
    {
    }

    IRenderPipeline* FGpuDeviceImpl::createRenderPipeline(const FRenderPipelineDesc& pipelineDesc)
    {
    }

    IComputePipeline* FGpuDeviceImpl::createComputePipeline(const FComputePipelineDesc& pipelineDesc)
    {
    }

    IFence* FGpuDeviceImpl::createFence(const FFenceDesc& fenceDesc)
    {
    }

    ISemaphore* FGpuDeviceImpl::createSemaphore(const FSemaphoreDesc& semaphoreDesc)
    {
    }

    ITextureView* FGpuDeviceImpl::getAcquiredSwapchainTextureView()
    {
    }

    void FGpuDeviceImpl::writeSamplerDescriptor(IBuffer* buffer, uint64_t offset, const ISampler* sampler)
    {
        IGpuDevice::writeSamplerDescriptor(buffer, offset, sampler);
    }

    void FGpuDeviceImpl::writeTextureDescriptor(IBuffer* buffer, uint64_t offset, const ITexture* texture,
        ETextureBindingType bindingType)
    {
        IGpuDevice::writeTextureDescriptor(buffer, offset, texture, bindingType);
    }

    void FGpuDeviceImpl::writeBufferDescriptor(IBuffer* buffer, uint64_t offset, const IBuffer* bufferResource,
        uint64_t resourceOffset, uint64_t resourceSize, EBufferBindingType bindingType)
    {
        IGpuDevice::writeBufferDescriptor(buffer, offset, bufferResource, resourceOffset, resourceSize, bindingType);
    }

    ID3D12CommandAllocator* FGpuDeviceImpl::getCommandAllocator(EQueueType queueType)
    {
        switch (queueType)
        {
        case EQueueType::None: return nullptr;
        case EQueueType::Render: return m_RenderCmdAllocator;
        case EQueueType::Compute: return m_ComputeCmdAllocator;
        case EQueueType::Copy: return m_CopyCmdAllocator;
        default: return nullptr;
        }
    }

    bool FGpuDeviceImpl::createCommandSignatures()
    {
        {
            D3D12_INDIRECT_ARGUMENT_DESC argDesc = {};
            argDesc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW;

            D3D12_COMMAND_SIGNATURE_DESC sigDesc = {};
            sigDesc.pArgumentDescs = &argDesc;
            sigDesc.NumArgumentDescs = 1;
            sigDesc.ByteStride = sizeof(D3D12_DRAW_ARGUMENTS);
            if (DX_FAILED(m_Handle->CreateCommandSignature(
                &sigDesc,
                nullptr,
                IID_PPV_ARGS(&m_DrawIndirectSignature)
            )))
                return false;
        }

        {
            D3D12_INDIRECT_ARGUMENT_DESC argDesc = {};
            argDesc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;

            D3D12_COMMAND_SIGNATURE_DESC sigDesc = {};
            sigDesc.pArgumentDescs = &argDesc;
            sigDesc.NumArgumentDescs = 1;
            sigDesc.ByteStride = sizeof(D3D12_DRAW_INDEXED_ARGUMENTS);
            if (DX_FAILED(m_Handle->CreateCommandSignature(
                &sigDesc,
                nullptr,
                IID_PPV_ARGS(&m_DrawIndexedIndirectSignature)
            )))
                return false;
        }

        {
            D3D12_INDIRECT_ARGUMENT_DESC argDesc = {};
            argDesc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH;

            D3D12_COMMAND_SIGNATURE_DESC sigDesc = {};
            sigDesc.pArgumentDescs = &argDesc;
            sigDesc.NumArgumentDescs = 1;
            sigDesc.ByteStride = sizeof(D3D12_DISPATCH_ARGUMENTS);
            if (DX_FAILED(m_Handle->CreateCommandSignature(
                &sigDesc,
                nullptr,
                IID_PPV_ARGS(&m_DispatchIndirectSignature)
            )))
                return false;
        }

        return true;
    }
}
