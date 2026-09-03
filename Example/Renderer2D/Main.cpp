#include <Luma/Memory/Ref.h>
#include <Luma/Runtime/DesktopWindow.h>
#include <Luma/Runtime/Flags.h>
#include <Luma/Runtime/Time.h>
#include <Luma/Rendering/GpuDevice.h>
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

    FGpuDeviceDesc gpuDeviceDesc;
    gpuDeviceDesc.deviceType = EGpuDeviceType::Vulkan;
    gpuDeviceDesc.buffering = ESwapchainBuffering::DoubleBuffering;
    gpuDeviceDesc.window = window;
    gpuDeviceDesc.vSync = true;
    Ref<IGpuDevice> gpuDevice = createGpuDevice(gpuDeviceDesc);

    Ref<FRenderer2D> renderer = Ref<FRenderer2D>::create(gpuDevice);

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

        if (gpuDevice->beginFrame())
        {
            ICommandBuffer* cmdBuffer = gpuDevice->getCommandBuffer();
            const ITextureView* swapchainTexture = gpuDevice->getAcquiredSwapchainTextureView();

            FRenderPassTarget colorAttachment;
            colorAttachment.type = ERenderPassTargetType::Color;
            colorAttachment.loadOp = ELoadOp::Clear;
            colorAttachment.storeOp = EStoreOp::Store;
            colorAttachment.clearValue.color = FColor::Black;
            colorAttachment.textureView = swapchainTexture;

            FRenderPassDesc renderPassDesc;
            renderPassDesc.renderArea = {0, 0, WIDTH, HEIGHT};
            renderPassDesc.colorTargets.add(&colorAttachment);

            cmdBuffer->beginRenderPass(renderPassDesc);
            renderer->render(cmdBuffer, WIDTH, HEIGHT);
            cmdBuffer->endRenderPass();

            gpuDevice->endFrame();
            gpuDevice->present();
        }
    }

    return 0;
}
