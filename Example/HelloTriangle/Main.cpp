#include <Luma/Memory/Ref.h>
#include <Luma/Runtime/Window.h>
#include <Luma/Runtime/Path.h>
#include <Luma/Rendering/Device.h>
#include <Luma/Rendering/Shader.h>
#include <Luma/Rendering/RenderPassDesc.h>
#include <Luma/Rendering/RenderPipeline.h>
#include <Luma/Rendering/CommandBuffer.h>

using namespace Luma;
using namespace Luma::RHI;

int main()
{
    Ref<Window> window = createWindow("Hello Triangle!", 800, 600, WindowOptions::Centered | WindowOptions::Resizable);
    Ref<Device> device = createDevice(window);

    const String vertexPath = Path::getAssetPath("Shaders/HelloTriangle.slang.vert.spv");
    const String fragmentPath = Path::getAssetPath("Shaders/HelloTriangle.slang.frag.spv");
    Ref<Shader> shaderProgram = device->createShader(vertexPath, fragmentPath);

    RenderPipelineDesc pipelineDesc;
    pipelineDesc.shaderProgram = shaderProgram;
    pipelineDesc.colorTargetCount = 1;
    pipelineDesc.colorFormats[0] = Format::RGBA8_SRGB;
    Ref<RenderPipeline> pipeline = device->createRenderPipeline(pipelineDesc);

    while (!window->shouldClose())
    {
        window->pollEvents();

        if (device->beginFrame())
        {
            CommandBuffer* cmdBuffer = device->getCommandBuffer();
            TextureView* swapchainTexture = device->getAcquiredSwapchainTextureView();

            RenderPassTarget colorTarget;
            colorTarget.type = RenderPassTargetType::Color;
            colorTarget.loadOp = LoadOp::Clear;
            colorTarget.storeOp = StoreOp::Store;
            colorTarget.clearValue.color = Color::Black;
            colorTarget.textureView = swapchainTexture;

            RenderPassDesc renderPassDesc;
            renderPassDesc.renderArea = window->getBounds();
            renderPassDesc.colorTargets.add(&colorTarget);

            cmdBuffer->beginRenderPass(renderPassDesc);
            cmdBuffer->bindRenderPipeline(pipeline);
            cmdBuffer->setViewport(Viewport::fromSize(window->getSize().as<float>()));
            cmdBuffer->setScissor(Scissor::fromSize(window->getSize()));
            cmdBuffer->draw(3, 1, 0, 0);
            cmdBuffer->endRenderPass();

            device->endFrame();
            device->present();
        }
    }

    device->waitIdle();
    return 0;
}
