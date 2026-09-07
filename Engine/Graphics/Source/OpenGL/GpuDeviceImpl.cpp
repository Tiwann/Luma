#include "Luma/OpenGL/GpuDeviceImpl.h"
#include "Luma/Runtime/DesktopWindow.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Luma::OpenGL
{
#if defined(LUMA_DEBUG) || defined(LUMA_DEV)
    void static OpenGLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,const GLchar *message, const void *userParam)
    {

    };
#endif
    
    EGpuDeviceType FGpuDeviceImpl::getDeviceType()
    {
        return EGpuDeviceType::OpenGL;
    }

    bool FGpuDeviceImpl::initialize(const FGpuDeviceDesc& deviceDesc)
    {
        if (!deviceDesc.window) return false;

        if (const FDesktopWindow* window = dynamic_cast<FDesktopWindow*>(deviceDesc.window))
            glfwMakeContextCurrent(window->getHandle());

        glfwSwapInterval(deviceDesc.vSync);

        if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
            return false;

#if defined(LUMA_DEBUG) || defined(LUMA_DEV)
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(OpenGLErrorCallback, nullptr);
#endif

        glEnable(GL_FRAMEBUFFER_SRGB);


        return true;
    }

    void FGpuDeviceImpl::destroy()
    {
    }

    bool FGpuDeviceImpl::beginFrame()
    {
        return true;
    }

    void FGpuDeviceImpl::endFrame()
    {
    }

    void FGpuDeviceImpl::present()
    {
    }

    void FGpuDeviceImpl::waitIdle()
    {
        glFinish();
    }

    uint32_t FGpuDeviceImpl::getTextureCount() const
    {
    }

    uint32_t FGpuDeviceImpl::getFrameIndex() const
    {
    }

    bool FGpuDeviceImpl::hasVSync()
    {
        return IGpuDevice::hasVSync();
    }

    void FGpuDeviceImpl::setVSync(bool enabled)
    {
        if (const FDesktopWindow* window = dynamic_cast<FDesktopWindow*>(m_Window))
            glfwMakeContextCurrent(window->getHandle());
        glfwSwapInterval(enabled ? 1 : 0);
    }

    ISwapchain* FGpuDeviceImpl::getSwapchain()
    {
        return IGpuDevice::getSwapchain();
    }

    IQueue* FGpuDeviceImpl::getRenderQueue()
    {
        return IGpuDevice::getRenderQueue();
    }

    IQueue* FGpuDeviceImpl::getComputeQueue()
    {
        return IGpuDevice::getComputeQueue();
    }

    IQueue* FGpuDeviceImpl::getCopyQueue()
    {
        return IGpuDevice::getCopyQueue();
    }

    IBuffer* FGpuDeviceImpl::createBuffer(const FBufferDesc& bufferDesc)
    {
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

    ITextureView* FGpuDeviceImpl::getAcquiredSwapchainTextureView()
    {
    }
}
