#include <Luma/Memory/Ref.h>
#include <Luma/Runtime/DesktopWindow.h>
#include <Luma/Runtime/Flags.h>
#include <Luma/Runtime/Path.h>
#include <Luma/Runtime/Time.h>
#include <Luma/Rendering/RenderDevice.h>
#include <Luma/Rendering/ShaderCompiler.h>
#include <Luma/Rendering/Shader.h>
#include <Luma/Rendering/CommandBuffer.h>
#include <Luma/Rendering/GraphicsPipeline.h>
#include <Luma/Rendering/RenderPassDesc.h>
#include <Luma/Rendering/Swapchain.h>

using namespace Luma;

int main()
{
    constexpr auto flags = TFlags(EWindowCreateBits::Centered);
    FWindowDesc windowDesc;
    windowDesc.title = "Hello Triangle";
    windowDesc.width = 800;
    windowDesc.height = 600;
    windowDesc.flags = flags;

    Ref<FDesktopWindow> window = createWindow(windowDesc);
    LUMA_ASSERT(window, "Failed to create window! Exiting application.");

    FRenderDeviceDesc renderDeviceDesc;
    renderDeviceDesc.deviceType = ERenderDeviceType::Vulkan;
    renderDeviceDesc.buffering = ESwapchainBuffering::DoubleBuffering;
    renderDeviceDesc.window = window;
    renderDeviceDesc.vSync = false;

    Ref<IRenderDevice> renderDevice = createRenderDevice(renderDeviceDesc);
    LUMA_ASSERT(renderDevice, "Render device failed to create! Exiting application.");
    window->resizedEvent.bind([&renderDevice](uint32_t, uint32_t)
    {
        ISwapchain* swapchain = renderDevice->getSwapchain();
        swapchain->invalidate();
    });

    FShaderDesc shaderDesc;
    shaderDesc.stageFlags = EShaderStageBits::Vertex | EShaderStageBits::Fragment;
    shaderDesc.moduleName = "HelloTriangle";
    shaderDesc.filepath = FPath::getAssetPath("Shaders/HelloTriangle.slang");

    Ref<IShader> shader = renderDevice->createShader(shaderDesc);
    LUMA_ASSERT(shader, "Failed to create shader! Exiting application.");

    FGraphicsPipelineDesc pipelineDesc;
    pipelineDesc.device = renderDevice;
    pipelineDesc.shaderProgram = shader;
    pipelineDesc.depthStencil.depthTestEnable = false;
    pipelineDesc.depthStencil.stencilTestEnable = false;
    pipelineDesc.depthStencil.depthWriteEnable = false;
    pipelineDesc.colorFormats[0] = EFormat::R8G8B8A8_SRGB;
    pipelineDesc.colorFormatCount = 1;

    Ref<IGraphicsPipeline> pipeline = renderDevice->createGraphicsPipeline(pipelineDesc);
    LUMA_ASSERT(pipeline, "Failed to create graphics pipeline! Exiting application.");

    while (!window->shouldClose())
    {
        window->pollEvents();

        if (renderDevice->beginFrame())
        {
            ICommandBuffer* cmdBuffer = renderDevice->getCommandBuffer();
            const ITextureView* swapchainTexture = renderDevice->getAcquiredSwapchainTextureView();

            FRenderPassAttachment colorAttachment;
            colorAttachment.type = ERenderPassAttachmentType::Color;
            colorAttachment.loadOp = ELoadOp::Clear;
            colorAttachment.storeOp = EStoreOp::Store;
            colorAttachment.clearValue.color = FColor::Black;
            colorAttachment.textureView = swapchainTexture;

            FRenderPassDesc renderPassDesc;
            renderPassDesc.renderArea = {0, 0, window->getWidth(), window->getHeight()};
            renderPassDesc.colorAttachments.add(&colorAttachment);

            cmdBuffer->beginRenderPass(renderPassDesc);
            cmdBuffer->bindGraphicsPipeline(pipeline);
            cmdBuffer->setViewport({0.0f, 0.0f, (float)window->getWidth(), (float)window->getHeight(), 0.0f, 1.0f});
            cmdBuffer->setScissor({0, 0, window->getWidth(), window->getHeight()});
            cmdBuffer->draw(3, 1, 0, 0);
            cmdBuffer->endRenderPass();

            renderDevice->endFrame();
            renderDevice->present();
        }
    }

    renderDevice->waitIdle();
    return 0;
}
