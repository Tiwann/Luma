#include <Luma/Memory/Ref.h>
#include <Luma/Runtime/Window.h>
#include <Luma/Runtime/Path.h>
#include <Luma/Rendering/Device.h>
#include <Luma/Rendering/Shader.h>
#include <Luma/Rendering/RenderGraph.h>
#include <Luma/Rendering/RenderPipeline.h>
#include <Luma/Rendering/CommandBuffer.h>

using namespace Luma;
using namespace Luma::RHI;

int main()
{
    Ref<Window> window = createWindow("Hello Triangle!", 800, 600, WindowOptions::Centered | WindowOptions::Resizable);
    Ref<Device> device = createDevice(window);

    const String vertexPath = Path::getAssetPath("Shaders/HelloTriangle.vert.spv");
    const String fragmentPath = Path::getAssetPath("Shaders/HelloTriangle.frag.spv");
    Ref<Shader> shaderProgram = device->createShader(vertexPath, fragmentPath);

    RenderPipelineDesc pipelineDesc;
    pipelineDesc.shaderProgram = shaderProgram;
    pipelineDesc.colorTargetCount = 1;
    pipelineDesc.colorFormats[0] = Format::RGBA8_SRGB;
    Ref<RenderPipeline> pipeline = device->createRenderPipeline(pipelineDesc);

    RenderGraph renderGraph(device);

    while (!window->shouldClose())
    {
        window->pollEvents();

        if (device->beginFrame())
        {
            renderGraph.reset();
            const RenderGraph::TextureHandle backbuffer = renderGraph.importTexture("Backbuffer", device->getAcquiredSwapchainTexture());

            // The graph begins the render pass and sets the viewport and scissor to the target's size
            renderGraph.addPass("Triangle",
                [&](RenderGraph::PassBuilder& builder)
                {
                    builder.addColorTarget(backbuffer, LoadOp::Clear, Color::Black);
                },
                [&](const RenderGraph::PassContext& context)
                {
                    CommandBuffer* cmdBuffer = context.getCommandBuffer();
                    cmdBuffer->bindRenderPipeline(pipeline);
                    cmdBuffer->draw(3, 1, 0, 0);
                });

            renderGraph.execute(device->getCommandBuffer());

            device->endFrame();
            device->present();
        }
    }

    device->waitIdle();
    return 0;
}
