#include "Luma/Deko3D/GpuDeviceImpl.h"
#include "Luma/Runtime/SwitchWindow.h"
#include <deko3d.h>

#include "Luma/Deko3D/BufferImpl.h"

namespace Luma::Deko3d
{
    EGpuDeviceType FGpuDeviceImpl::getDeviceType()
    {
        return EGpuDeviceType::Deko3D;
    }

    static void debugCallback(void* userData, const char* context, DkResult result, const char* message)
    {
        std::cout << "[" << context << "] " << "An error has occurred: " << message;
    };

    bool FGpuDeviceImpl::initialize(const FGpuDeviceDesc& deviceDesc)
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

    void FGpuDeviceImpl::destroy()
    {
        m_CopyQueue.destroy();
        m_ComputeQueue.destroy();
        m_RenderQueue.destroy();
        m_Swapchain.destroy();
        dkDeviceDestroy(m_Device);
    }

    bool FGpuDeviceImpl::beginFrame()
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

    void FGpuDeviceImpl::endFrame()
    {
        FCommandBufferImpl& cmdBuff = m_CmdBuffers[m_SwapchainImageIndex];
        cmdBuff.end();

        m_RenderQueue.executeCommandBuffer(&cmdBuff, nullptr);
    }

    void FGpuDeviceImpl::present()
    {
        if (!m_RenderQueue.present(&m_Swapchain, nullptr, m_SwapchainImageIndex))
            m_Swapchain.invalidate();
        m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % m_Swapchain.getImageCount();
    }

    void FGpuDeviceImpl::waitIdle()
    {
        m_RenderQueue.waitIdle();
        m_ComputeQueue.waitIdle();
        m_CopyQueue.waitIdle();
    }

    uint32_t FGpuDeviceImpl::getFrameCount()
    {
        return m_Swapchain.getImageCount();
    }

    uint32_t FGpuDeviceImpl::getCurrentFrameIndex()
    {
        return m_CurrentFrameIndex;
    }

    IBuffer* FGpuDeviceImpl::createBuffer(const FBufferDesc& bufferDesc)
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

    ITexture* FGpuDeviceImpl::createTexture(const FTextureDesc& textureDesc)
    {
    }

    ITextureView* FGpuDeviceImpl::createTextureView(const FTextureViewDesc& textureViewDesc)
    {
    }

    IShader* FGpuDeviceImpl::createShader(const FShaderDesc& shaderDesc)
    {
    }

    ICommandBuffer* FGpuDeviceImpl::createCommandBuffer(const FCommandBufferDesc& cmdBufferDesc)
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

    ICommandBuffer* FGpuDeviceImpl::getCommandBuffer()
    {
        return &m_CmdBuffers[m_CurrentFrameIndex];
    }

    ISampler* FGpuDeviceImpl::createSampler(const FSamplerDesc& samplerDesc)
    {
    }

    IGraphicsPipeline* FGpuDeviceImpl::createGraphicsPipeline(
        const FGraphicsPipelineDesc& pipelineDesc)
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

    DkDevice FGpuDeviceImpl::getHandle() const
    {
        return m_Device;
    }

    FSwitchWindow* FGpuDeviceImpl::getWindow() const
    {
        return m_Window;
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
}
