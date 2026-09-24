#include <Luma/Containers/StringFormat.h>
#include <Luma/Input/Immediate.h>
#include <Luma/Memory/Ref.h>
#include <Luma/Rendering/CommandBuffer.h>
#include <Luma/Rendering/Device.h>
#include <Luma/Rendering/Renderer2D.h>
#include <Luma/Rendering/RenderPassDesc.h>
#include <Luma/Runtime/Time.h>
#include <Luma/Runtime/Window.h>


using namespace Luma;
using namespace Luma::RHI;

static constexpr uint32_t kWidth = 800;
static constexpr uint32_t kHeight = 600;

int main()
{
    Ref<Window> window = createWindow("Hello Renderer2D", kWidth, kHeight, WindowOptions::Centered);
    Ref<Device> device = createDevice(window);
    Ref<Renderer2D> renderer = Ref<Renderer2D>::create(device, kWidth, kHeight);

    Camera camera;
    camera.setSize(kWidth, kHeight);
    camera.setProjectionMode(CameraProjectionMode::Orthographic);
    camera.setClipPlanes(-1.0f, 1.0f);

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

        Ref<Texture> renderedTexture = renderer->render(camera);

        if (device->beginFrame())
        {
            CommandBuffer* cmdBuffer = device->getCommandBuffer();

            device->endFrame();
            device->present();
        }
    }

    return 0;
}
