#include <Luma/Memory/Ref.h>
#include <Luma/Runtime/Window.h>
#include <Luma/Runtime/Path.h>
#include <Luma/Rendering/GPUDevice.h>
#include <Luma/Rendering/Shader.h>
#include <Luma/Rendering/RenderPassDesc.h>
#include <Luma/Rendering/RenderPipeline.h>
#include <Luma/Rendering/CommandBuffer.h>

using namespace Luma;

int main()
{
    Ref<IWindow> window = createWindow("Hello Triangle!", 800, 600, EWindowOptions::Centered | EWindowOptions::Resizable);
    LUMA_ASSERT(window, "Failed to create window! Exiting application.");

    Ref<IGPUDevice> device = createGPUDevice(window);
    LUMA_ASSERT(device, "Render device failed to create! Exiting application.");

    const FString vertexPath = FPath::getAssetPath("Shaders/HelloTriangle.slang.vert.spv");
    const FString fragmentPath = FPath::getAssetPath("Shaders/HelloTriangle.slang.frag.spv");
    Ref<IShader> shaderProgram = device->createShader(vertexPath, fragmentPath);
    LUMA_ASSERT(device, "Shader program failed to create! Exiting application.");

    FRenderPipelineDesc pipelineDesc;
    pipelineDesc.shaderProgram = shaderProgram;
    pipelineDesc.colorTargetCount = 1;
    pipelineDesc.colorFormats[0] = EFormat::RGBA8_SRGB;

    Ref<IRenderPipeline> pipeline = device->createRenderPipeline(pipelineDesc);
    LUMA_ASSERT(pipeline, "Failed to create graphics pipeline! Exiting application.");

    while (!window->shouldClose())
    {
        window->pollEvents();

        if (device->beginFrame())
        {
            ICommandBuffer* cmdBuffer = device->getCommandBuffer();
            ITextureView* swapchainTexture = device->getAcquiredSwapchainTextureView();

            FRenderPassTarget colorTarget;
            colorTarget.type = ERenderPassTargetType::Color;
            colorTarget.loadOp = ELoadOp::Clear;
            colorTarget.storeOp = EStoreOp::Store;
            colorTarget.clearValue.color = FColor::Black;
            colorTarget.textureView = swapchainTexture;

            FRenderPassDesc renderPassDesc;
            renderPassDesc.renderArea = window->getBounds();
            renderPassDesc.colorTargets.add(&colorTarget);

            cmdBuffer->beginRenderPass(renderPassDesc);
            cmdBuffer->bindRenderPipeline(pipeline);
            cmdBuffer->setViewport(FViewport(0.0f, 0.0f, (float)window->getWidth(), (float)window->getHeight(), 0.0f, 1.0f));
            cmdBuffer->setScissor(FScissor(0, 0, window->getWidth(), window->getHeight()));
            cmdBuffer->draw(3, 1, 0, 0);
            cmdBuffer->endRenderPass();

            device->endFrame();
            device->present();
        }
    }

    device->waitIdle();
    return 0;
}
