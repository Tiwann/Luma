#include <Luma/Memory/Ref.h>
#include <Luma/Runtime/DesktopWindow.h>

#include <Luma/Runtime/Time.h>
#include <Luma/Rendering/GPUDevice.h>
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
    Ref<FDesktopWindow> window = createWindow("Hello Renderer2D", WIDTH, HEIGHT, EWindowOptions::Centered);
    Ref<IGPUDevice> gpuDevice = createGPUDevice(window);
    Ref<FRenderer2D> renderer = Ref<FRenderer2D>::create(gpuDevice);


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
            renderer->render();
            cmdBuffer->endRenderPass();

            gpuDevice->endFrame();
            gpuDevice->present();
        }
    }

    return 0;
}
