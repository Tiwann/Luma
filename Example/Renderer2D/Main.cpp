#include <Luma/Memory/Ref.h>
#include <Luma/Runtime/DesktopWindow.h>
#include <Luma/Runtime/Flags.h>
#include <Luma/Runtime/Time.h>
#include <Luma/Rendering/RenderDevice.h>
#include <Luma/Rendering/ShaderCompiler.h>
#include <Luma/Rendering/CommandBuffer.h>
#include <Luma/Rendering/RenderPassDesc.h>
#include <Luma/Containers/StringFormat.h>
#include <Luma/Input/Immediate.h>
#include <Luma/Rendering/Renderer2D.h>

using namespace Luma;

static constexpr uint32_t WIDTH = 800;
static constexpr uint32_t HEIGHT = 600;

int main()
{
    const FWindowDesc windowDesc { "Hello Triangle", WIDTH, HEIGHT, EWindowCreateBits::Centered };
    Ref<FDesktopWindow> window = createWindow(windowDesc);

    FRenderDeviceDesc renderDeviceDesc;
    renderDeviceDesc.deviceType = ERenderDeviceType::Vulkan;
    renderDeviceDesc.buffering = ESwapchainBuffering::DoubleBuffering;
    renderDeviceDesc.window = window;
    renderDeviceDesc.vSync = true;
    Ref<IRenderDevice> renderDevice = createRenderDevice(renderDeviceDesc);

    Ref<FRenderer2D> renderer = Ref<FRenderer2D>::create();
    renderer->initialize(renderDevice);

    FTime::initialize();

    float lastTime = 0.0f;
    while (!window->shouldClose())
    {
        window->pollEvents();
        const double currentTime = FTime::getTime();
        const float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        renderer->begin();
        renderer->drawText(strfmt("DeltaTime: {:.3f}ms", deltaTime), {0, 0}, 20, FColor::Cyan);
        renderer->end();

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
            renderer->render(cmdBuffer, WIDTH, HEIGHT);
            cmdBuffer->endRenderPass();

            renderDevice->endFrame();
            renderDevice->present();
        }
    }

    return 0;
}
