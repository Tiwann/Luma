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

static constexpr float PIXELS_PER_METER = 64.0f;
static constexpr float toPixels(float value) { return value * PIXELS_PER_METER; }
static constexpr FVector3f GRAVITY {0.0f, toPixels(-9.81f), 0.0f};

int main(int argc, const char** argv)
{
    Ref<Window> window = createWindow("Hello Triangle!", 800, 600, WindowOptions::Centered | WindowOptions::Resizable);
    Ref<Device> device = createGPUDevice(window);
    Ref<FPhysicsWorld> world = createPhysicsWorld(FPhysicsWorldDesc(GRAVITY));
    Ref<FRenderer2D> renderer = Ref<FRenderer2D>::create(device, 800, 600, SampleCount1x);
    window->resizedEvent.bindMember(renderer.get(), &FRenderer2D::resize);

    const FVector2f bodySize = FVector2f(20, 20);
    Ref<FPhysicsBody> body = world->createBody();
    body->setBodyType(EPhysicsBodyType::Dynamic);
    body->setConstraints(EPhysicsConstraints::PositionZ);

    const FVector2f floorSize = FVector2f(300, 30);
    Ref<FPhysicsBody> floorBody = world->createBody();
    floorBody->setBodyType(EPhysicsBodyType::Static);
    floorBody->setPosition(FVector3f(0.0f, -100.0f, 0.0f));

    Ref<FBoxShape> floorBox = Ref<FBoxShape>::create();
    floorBox->setSize(FVector3f(floorSize / 2, 150));
    floorBody->attachShape(floorBox);

    Ref<FBoxShape> box = Ref<FBoxShape>::create();
    body->attachShape(box);
    box->setSize(FVector3f(bodySize / 2, 10));

    FullscreenRenderPass fullscreenPass(device, Format::RGBA8_SRGB);

    while (!window->shouldClose())
    {
        window->pollEvents();
        world->step();

        const auto& camera = renderer->getCamera();
        const auto position = body->getPosition();
        const auto screenSpacePosition = camera.worldToScreen(position);

        renderer->begin();
        renderer->drawText(strfmt("Position : ({:.2f}, {:.2f})", position.x, position.y), {0.0, 0.0}, 18.0f, Color::White);
        renderer->drawQuad(screenSpacePosition - bodySize / 2, bodySize, 0, Color::Blue);
        renderer->drawQuad(camera.worldToScreen(floorBody->getPosition()) - floorSize / 2, floorSize, 0, Color::Red);
        renderer->end();

        Ref<Texture> rendererTexture = renderer->render();
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
