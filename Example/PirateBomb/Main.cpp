#include "FullscreenRenderPass.h"
#include "Luma/Containers/StringFormat.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Physics/BoxShape.h"
#include "Luma/Runtime/Window.h"
#include "Luma/Rendering/Device.h"
#include "Luma/Rendering/RenderPassDesc.h"
#include "Luma/Rendering/Renderer2D.h"
#include "Luma/Physics/PhysicsWorld.h"

using namespace Luma;
using namespace Luma::RHI;

static constexpr float kPixelsPerMeter = 64.0f;
static constexpr FVector3f kGravity{0.0f, kPixelsPerMeter * 9.81, 0.0f};
static constexpr uint32_t kWidth = 800;
static constexpr uint32_t kHeight = 600;

int main()
{
    Ref<Window> window = createWindow("Hello Triangle!", kWidth, kHeight, WindowOptions::Centered);
    Ref<Device> device = createDevice(window);
    Ref<PhysicsWorld> world = createPhysicsWorld({kGravity});
    Ref<Renderer2D> renderer = Ref<Renderer2D>::create(device, kWidth, kHeight);
    window->resizedEvent.bindMember(renderer.get(), &Renderer2D::resize);

    Camera camera;
    camera.setSize(kWidth, kHeight);
    camera.setProjectionMode(CameraProjectionMode::Orthographic);
    camera.setClipPlanes(-1.0f, 1.0f);

    const FVector2f bodySize = FVector2f(20, 20);
    Ref<PhysicsBody> body = world->createBody();
    body->setBodyType(EPhysicsBodyType::Dynamic);
    body->setConstraints(EPhysicsConstraints::PositionZ);

    const FVector2f floorSize = FVector2f(300, 30);
    Ref<PhysicsBody> floorBody = world->createBody();
    floorBody->setBodyType(EPhysicsBodyType::Static);
    floorBody->setPosition(FVector3f(0.0f, -100.0f, 0.0f));

    Ref<BoxShape> floorBox = Ref<BoxShape>::create();
    floorBox->setSize(FVector3f(floorSize / 2, 150));
    floorBody->attachShape(floorBox);

    Ref<BoxShape> box = Ref<BoxShape>::create();
    body->attachShape(box);
    box->setSize(FVector3f(bodySize / 2, 10));

    FullscreenRenderPass fullscreenPass(device, Format::RGBA8_SRGB);

    while (!window->shouldClose())
    {
        window->pollEvents();
        world->step();

        const auto position = body->getPosition();
        const auto screenSpacePosition = camera.worldToScreen(position);

        renderer->begin();
        renderer->drawText(strfmt("Position : ({:.2f}, {:.2f})", position.x, position.y), {0.0, 0.0}, 18.0f, Color::White);
        renderer->drawQuad(screenSpacePosition - bodySize / 2, bodySize, 0, Color::Blue);
        renderer->drawQuad(camera.worldToScreen(floorBody->getPosition()) - floorSize / 2, floorSize, 0, Color::Red);
        renderer->end();

        Ref<Texture> rendererTexture = renderer->render(camera);
        fullscreenPass.setInputTexture(rendererTexture);

        if (device->beginFrame())
        {
            fullscreenPass.setOutputTexture(device->getAcquiredSwapchainTexture());
            fullscreenPass.setSize(window->getSize());
            fullscreenPass.execute(device->getCommandBuffer());
            device->endFrame();
            device->present();
        }
    }

    device->waitIdle();
    return 0;
}
