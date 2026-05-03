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
#include <Luma/Rendering/TextureUtils.h>
#include <Luma/Asset/Material.h>

#include "Luma/Rendering/Renderer2D.h"

using namespace Luma;

static constexpr uint32_t WIDTH = 800;
static constexpr uint32_t HEIGHT = 600;

int main()
{
    constexpr auto flags = EWindowCreateBits::Centered;
    FWindowDesc windowDesc;
    windowDesc.title = "Hello Triangle";
    windowDesc.width = WIDTH;
    windowDesc.height = HEIGHT;
    windowDesc.flags = flags;

    Ref<FDesktopWindow> window = createWindow(windowDesc);
    LUMA_ASSERT(window, "Failed to create window! Exiting application.");

    FRenderDeviceDesc renderDeviceDesc;
    renderDeviceDesc.deviceType = ERenderDeviceType::Vulkan;
    renderDeviceDesc.buffering = ESwapchainBuffering::DoubleBuffering;
    renderDeviceDesc.window = window;
    renderDeviceDesc.vSync = true;

    Ref<IRenderDevice> renderDevice = createRenderDevice(renderDeviceDesc);
    LUMA_ASSERT(renderDevice, "Render device failed to create! Exiting application.");
    window->resizedEvent.bind([&renderDevice](uint32_t, uint32_t)
    {
        ISwapchain* swapchain = renderDevice->getSwapchain();
        swapchain->invalidate();
    });

    FRenderer2D renderer;
    if (!renderer.initialize(renderDevice))
        LUMA_ASSERT(false, "Failed to initialize renderer 2d! Exiting application.");

    while (!window->shouldClose())
    {
        window->pollEvents();

        renderer.begin();
        renderer.drawQuad(FRect2f(0, 0, 200, 200), 0.0f, FColor::White);
        renderer.end();

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
            renderPassDesc.renderArea = {0, 0, WIDTH, HEIGHT};
            renderPassDesc.colorAttachments.add(&colorAttachment);

            cmdBuffer->beginRenderPass(renderPassDesc);
            renderer.render(cmdBuffer, WIDTH, HEIGHT);
            cmdBuffer->endRenderPass();

            renderDevice->endFrame();
            renderDevice->present();
        }
    }

    renderer.destroy();
    renderDevice->waitIdle();
    return 0;
}
