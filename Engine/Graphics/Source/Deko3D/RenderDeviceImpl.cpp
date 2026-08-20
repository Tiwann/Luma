#include "Luma/Deko3D/RenderDeviceImpl.h"
#include "Luma/Runtime/SwitchWindow.h"
#include <deko3d.h>

#include "Luma/Deko3D/BufferImpl.h"

namespace Luma::Deko3d
{
    ERenderDeviceType FRenderDeviceImpl::getDeviceType()
    {
        return ERenderDeviceType::Deko3D;
    }

    static void debugCallback(void* userData, const char* context, DkResult result, const char* message)
    {
        std::cout << "[" << context << "] " << "An error has occurred: " << message;
    };

    bool FRenderDeviceImpl::initialize(const FRenderDeviceDesc& deviceDesc)
    {
        DkDeviceMaker createInfo;
        createInfo.userData = this;
        createInfo.flags = (DkDeviceFlags_DepthZeroToOne | DkDeviceFlags_OriginUpperLeft);
        createInfo.cbDebug = debugCallback;
        m_Device = dkDeviceCreate(&createInfo);
        if (!m_Device) return false;

        m_Window = static_cast<FSwitchWindow*>(deviceDesc.window);

        FSwapchainDesc desc;
        desc.device = this;
        desc.buffering = deviceDesc.buffering;
        desc.format = EFormat::R8G8B8A8_UNORM;
        desc.width = m_Window->getWidth();
        desc.height = m_Window->getHeight();
        desc.presentMode = EPresentMode::Unknown;

        if (!m_Swapchain.initialize(desc))
            return false;

        m_RenderQueue.initialize(this, EQueueType::Render);
        m_ComputeQueue.initialize(this, EQueueType::Compute);
        m_CopyQueue.initialize(this, EQueueType::Copy);


        return true;
    }

    void FRenderDeviceImpl::destroy()
    {
        m_CopyQueue.destroy();
        m_ComputeQueue.destroy();
        m_RenderQueue.destroy();
        m_Swapchain.destroy();
        dkDeviceDestroy(m_Device);
    }

    bool FRenderDeviceImpl::beginFrame()
    {
        if (!m_Window->isFocused())
            return false;

        if (!m_Swapchain.isValid())
        {
            waitIdle();
            m_Swapchain.resize(m_Window->getWidth(), m_Window->getHeight());
            m_CurrentFrameIndex = 0;
        }

        if (!m_Swapchain.acquireNextImage(nullptr, m_SwapchainImageIndex))
        {
            m_Swapchain.invalidate();
            return false;
        }

        FCommandBufferImpl& cmdBuff = m_CmdBuffers[m_SwapchainImageIndex];
        cmdBuff.begin();

        return true;
    }

    void FRenderDeviceImpl::endFrame()
    {
        FCommandBufferImpl& cmdBuff = m_CmdBuffers[m_SwapchainImageIndex];
        cmdBuff.end();

        m_RenderQueue.executeCommandBuffer(&cmdBuff, nullptr);
    }

    void FRenderDeviceImpl::present()
    {
        if (!m_RenderQueue.present(&m_Swapchain, nullptr, m_SwapchainImageIndex))
            m_Swapchain.invalidate();
        m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % m_Swapchain.getImageCount();
    }

    void FRenderDeviceImpl::waitIdle()
    {
        m_RenderQueue.waitIdle();
        m_ComputeQueue.waitIdle();
        m_CopyQueue.waitIdle();
    }

    uint32_t FRenderDeviceImpl::getFrameCount()
    {
        return m_Swapchain.getImageCount();
    }

    uint32_t FRenderDeviceImpl::getCurrentFrameIndex()
    {
        return m_CurrentFrameIndex;
    }

    IBuffer* FRenderDeviceImpl::createBuffer(const FBufferDesc& bufferDesc)
    {
        FBufferDesc desc(bufferDesc);
        desc.device = this;
        FBufferImpl* buffer = new FBufferImpl();
        if (!buffer->initialize(desc))
        {
            delete buffer;
            return nullptr;
        }
        return buffer;
    }

    ITexture* FRenderDeviceImpl::createTexture(const FTextureDesc& textureDesc)
    {
    }

    ITextureView* FRenderDeviceImpl::createTextureView(const FTextureViewDesc& textureViewDesc)
    {
    }

    IShader* FRenderDeviceImpl::createShader(const FShaderDesc& shaderDesc)
    {
    }

    ICommandBuffer* FRenderDeviceImpl::createCommandBuffer(const FCommandBufferDesc& cmdBufferDesc)
    {
        FCommandBufferDesc desc(cmdBufferDesc);
        desc.device = this;
        FCommandBufferImpl* cmdBuffer = new FCommandBufferImpl();
        if (!cmdBuffer->initialize(desc))
        {
            delete cmdBuffer;
            return nullptr;
        }
        return cmdBuffer;
    }

    ICommandBuffer* FRenderDeviceImpl::getCommandBuffer()
    {
        return &m_CmdBuffers[m_CurrentFrameIndex];
    }

    ISampler* FRenderDeviceImpl::createSampler(const FSamplerDesc& samplerDesc)
    {
    }

    IGraphicsPipeline* FRenderDeviceImpl::createGraphicsPipeline(
        const FGraphicsPipelineDesc& pipelineDesc)
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

    DkDevice FRenderDeviceImpl::getHandle() const
    {
        return m_Device;
    }

    FSwitchWindow* FRenderDeviceImpl::getWindow() const
    {
        return m_Window;
    }

    IQueue* FRenderDeviceImpl::getRenderQueue()
    {
        return &m_RenderQueue;
    }

    IQueue* FRenderDeviceImpl::getComputeQueue()
    {
        return &m_ComputeQueue;
    }

    IQueue* FRenderDeviceImpl::getCopyQueue()
    {
        return &m_CopyQueue;
    }
}
