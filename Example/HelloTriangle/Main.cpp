#include <Luma/Memory/Ref.h>
#include <Luma/Runtime/Window.h>
#include <Luma/Runtime/Path.h>
#include <Luma/Rendering/RenderDevice.h>
#include <Luma/Rendering/ShaderProgram.h>
#include <Luma/Rendering/CommandBuffer.h>
#include <Luma/Rendering/RenderPipeline.h>
#include <Luma/Rendering/RenderPassDesc.h>
#include <Luma/Rendering/Swapchain.h>

#include "Luma/Rendering/Fence.h"

using namespace Luma;

int main()
{
    Ref<IWindow> window = createWindow({"Hello Triangle!", 800, 600, EWindowCreateBits::Centered | EWindowCreateBits::Resizable});
    LUMA_ASSERT(window, "Failed to create window! Exiting application.");

    FRenderDeviceDesc renderDeviceDesc;
    renderDeviceDesc.window = window;
    renderDeviceDesc.deviceType = ERenderDeviceType::Auto;
    renderDeviceDesc.buffering = ESwapchainBuffering::TripleBuffering;
    renderDeviceDesc.vSync = false;

    Ref<IRenderDevice> renderDevice = createRenderDevice(renderDeviceDesc);
    LUMA_ASSERT(renderDevice, "Render device failed to create! Exiting application.");

    /*Ref<IFence> fence = renderDevice->createFence();



    Ref<IShaderProgram> vertexShader = renderDevice->createShader(FPath::getAssetPath("Shaders/HelloTriangle.slang.vert.spv"));
    Ref<IShaderProgram> fragmentShader = renderDevice->createShader(FPath::getAssetPath("Shaders/HelloTriangle.slang.frag.spv"));

    FRenderPipelineDesc pipelineDesc;
    pipelineDesc.vertexShader = vertexShader;
    pipelineDesc.fragmentShader = fragmentShader;
    pipelineDesc.colorFormats[0] = EFormat::R8G8B8A8_SRGB;
    pipelineDesc.colorFormatCount = 1;

    Ref<IRenderPipeline> pipeline = renderDevice->createRenderPipeline(pipelineDesc);
    LUMA_ASSERT(pipeline, "Failed to create graphics pipeline! Exiting application.");*/

    while (!window->shouldClose())
    {
        window->pollEvents();

        if (renderDevice->beginFrame())
        {
            ICommandBuffer* cmdBuffer = renderDevice->getCommandBuffer();
            const ITextureView* swapchainTexture = renderDevice->getAcquiredSwapchainTextureView();

            FRenderPassTarget colorTarget;
            colorTarget.type = ERenderPassTargetType::Color;
            colorTarget.loadOp = ELoadOp::Clear;
            colorTarget.storeOp = EStoreOp::Store;
            colorTarget.clearValue.color = FColor::Black;
            colorTarget.textureView = swapchainTexture;

            FRenderPassDesc renderPassDesc;
            renderPassDesc.renderArea = window->getBounds();
            renderPassDesc.colorTargets.add(&colorTarget);

            /*cmdBuffer->beginRenderPass(renderPassDesc);
            cmdBuffer->bindRenderPipeline(pipeline);
            cmdBuffer->setViewport({0.0f, 0.0f, (float)window->getWidth(), (float)window->getHeight(), 0.0f, 1.0f});
            cmdBuffer->setScissor({0, 0, window->getWidth(), window->getHeight()});
            cmdBuffer->draw(3, 1, 0, 0);
            cmdBuffer->endRenderPass();*/

            renderDevice->endFrame();
            renderDevice->present();
        }
    }

    renderDevice->waitIdle();
    return 0;
}
