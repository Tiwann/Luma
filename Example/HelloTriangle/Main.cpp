#include <Luma/Memory/Ref.h>
#include <Luma/Runtime/Window.h>
#include <Luma/Runtime/Path.h>
#include <Luma/Rendering/GpuDevice.h>
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

    FGpuDeviceDesc gpuDeviceDesc;
    gpuDeviceDesc.window = window;
    gpuDeviceDesc.deviceType = EGpuDeviceType::Auto;
    gpuDeviceDesc.buffering = ESwapchainBuffering::TripleBuffering;
    gpuDeviceDesc.vSync = false;

    Ref<IGpuDevice> gpuDevice = createGpuDevice(gpuDeviceDesc);
    LUMA_ASSERT(gpuDevice, "Render device failed to create! Exiting application.");

    /*Ref<IFence> fence = gpuDevice->createFence();



    Ref<IShaderProgram> vertexShader = gpuDevice->createShader(FPath::getAssetPath("Shaders/HelloTriangle.slang.vert.spv"));
    Ref<IShaderProgram> fragmentShader = gpuDevice->createShader(FPath::getAssetPath("Shaders/HelloTriangle.slang.frag.spv"));

    FRenderPipelineDesc pipelineDesc;
    pipelineDesc.vertexShader = vertexShader;
    pipelineDesc.fragmentShader = fragmentShader;
    pipelineDesc.colorFormats[0] = EFormat::R8G8B8A8_SRGB;
    pipelineDesc.colorFormatCount = 1;

    Ref<IRenderPipeline> pipeline = gpuDevice->createRenderPipeline(pipelineDesc);
    LUMA_ASSERT(pipeline, "Failed to create graphics pipeline! Exiting application.");*/

    while (!window->shouldClose())
    {
        window->pollEvents();

        if (gpuDevice->beginFrame())
        {
            ICommandBuffer* cmdBuffer = gpuDevice->getCommandBuffer();
            const ITextureView* swapchainTexture = gpuDevice->getAcquiredSwapchainTextureView();

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

            gpuDevice->endFrame();
            gpuDevice->present();
        }
    }

    gpuDevice->waitIdle();
    return 0;
}
