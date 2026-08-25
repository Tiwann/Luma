#include "Luma/D3D12/RenderDeviceImpl.h"
#include "Luma/Runtime/DesktopWindow.h"
#include <directx/d3d12.h>
#include <dxgi1_6.h>

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
    
    ERenderDeviceType FRenderDeviceImpl::getDeviceType()
    {
        return ERenderDeviceType::D3D12;
    }

    bool FRenderDeviceImpl::initialize(const FRenderDeviceDesc& deviceDesc)
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
    }

    void FRenderDeviceImpl::destroy()
    {
    }

    bool FRenderDeviceImpl::beginFrame()
    {
    }

    void FRenderDeviceImpl::endFrame()
    {
    }

    void FRenderDeviceImpl::present()
    {
    }

    void FRenderDeviceImpl::waitIdle()
    {
    }

    uint32_t FRenderDeviceImpl::getFrameCount() const
    {
    }

    uint32_t FRenderDeviceImpl::getCurrentFrameIndex()
    {
    }

    bool FRenderDeviceImpl::hasVSync()
    {
        return IRenderDevice::hasVSync();
    }

    ISwapchain* FRenderDeviceImpl::getSwapchain()
    {
        return IRenderDevice::getSwapchain();
    }

    IQueue* FRenderDeviceImpl::getRenderQueue()
    {
        return IRenderDevice::getRenderQueue();
    }

    IQueue* FRenderDeviceImpl::getComputeQueue()
    {
        return IRenderDevice::getComputeQueue();
    }

    IQueue* FRenderDeviceImpl::getCopyQueue()
    {
        return IRenderDevice::getCopyQueue();
    }

    IBuffer* FRenderDeviceImpl::createBuffer(const FBufferDesc& bufferDesc)
    {
    }

    ITexture* FRenderDeviceImpl::createTexture(const FTextureDesc& textureDesc)
    {
    }

    ITextureView* FRenderDeviceImpl::createTextureView(const FTextureViewDesc& textureViewDesc)
    {
    }

    IShaderProgram* FRenderDeviceImpl::createShader(const FShaderDesc& shaderDesc)
    {
    }

    ICommandBuffer* FRenderDeviceImpl::createCommandBuffer(const FCommandBufferDesc& cmdBufferDesc)
    {
    }

    ICommandBuffer* FRenderDeviceImpl::getCommandBuffer()
    {
    }

    ISampler* FRenderDeviceImpl::createSampler(const FSamplerDesc& samplerDesc)
    {
    }

    IRenderPipeline* FRenderDeviceImpl::createRenderPipeline(const FRenderPipelineDesc& pipelineDesc)
    {
    }

    IComputePipeline* FRenderDeviceImpl::createComputePipeline(const FComputePipelineDesc& pipelineDesc)
    {
    }

    IFence* FRenderDeviceImpl::createFence(const FFenceDesc& fenceDesc)
    {
    }

    ISemaphore* FRenderDeviceImpl::createSemaphore(const FSemaphoreDesc& semaphoreDesc)
    {
    }

    ITextureView* FRenderDeviceImpl::getAcquiredSwapchainTextureView()
    {
    }

    void FRenderDeviceImpl::writeSamplerDescriptor(IBuffer* buffer, uint64_t offset, const ISampler* sampler)
    {
        IRenderDevice::writeSamplerDescriptor(buffer, offset, sampler);
    }

    void FRenderDeviceImpl::writeTextureDescriptor(IBuffer* buffer, uint64_t offset, const ITexture* texture,
        ETextureBindingType bindingType)
    {
        IRenderDevice::writeTextureDescriptor(buffer, offset, texture, bindingType);
    }

    void FRenderDeviceImpl::writeBufferDescriptor(IBuffer* buffer, uint64_t offset, const IBuffer* bufferResource,
        uint64_t resourceOffset, uint64_t resourceSize, EBufferBindingType bindingType)
    {
        IRenderDevice::writeBufferDescriptor(buffer, offset, bufferResource, resourceOffset, resourceSize, bindingType);
    }

    ID3D12CommandAllocator* FRenderDeviceImpl::getCommandAllocator(EQueueType queueType)
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

    bool FRenderDeviceImpl::createCommandSignatures()
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
