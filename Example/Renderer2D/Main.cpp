#include <Luma/Memory/Ref.h>
#include <Luma/Runtime/DesktopWindow.h>

#include <Luma/Runtime/Time.h>
#include <Luma/Rendering/Device.h>
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
    Ref<DesktopWindow> window = createWindow("Hello Renderer2D", WIDTH, HEIGHT, WindowOptions::Centered);
    Ref<Device> gpuDevice = createGPUDevice(window);
    Ref<FRenderer2D> renderer = Ref<FRenderer2D>::create(gpuDevice);


    float lastTime = 0.0f;
    while (!window->shouldClose())
    {
        window->pollEvents();
        const double currentTime = FTime::getTime();
        const float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        renderer->begin();
        renderer->drawText(strfmt("DeltaTime: {:.3f}ms", deltaTime), {0, 0}, 20, Color::Cyan);
        renderer->end();

        if (gpuDevice->beginFrame())
        {
            CommandBuffer* cmdBuffer = gpuDevice->getCommandBuffer();
            const ITextureView* swapchainTexture = gpuDevice->getAcquiredSwapchainTextureView();

            RenderPassTarget colorAttachment;
            colorAttachment.type = RenderPassTargetType::Color;
            colorAttachment.loadOp = LoadOp::Clear;
            colorAttachment.storeOp = StoreOp::Store;
            colorAttachment.clearValue.color = Color::Black;
            colorAttachment.textureView = swapchainTexture;

            RenderPassDesc renderPassDesc;
            renderPassDesc.renderArea = {0, 0, WIDTH, HEIGHT};
            renderPassDesc.colorTargets.add(&colorAttachment);

            cmdBuffer->beginRenderPass(renderPassDesc);
            renderer->render();
            cmdBuffer->endRenderPass();

            gpuDevice->endFrame();
            gpuDevice->present();
        }
    }

    return 0;
}
