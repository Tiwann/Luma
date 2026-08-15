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

#include "Luma/Physics/PhysicsWorld.h"
#include "Luma/Asset/Font.h"
#include "Luma/Containers/StringFormat.h"
#include "Luma/Input/Immediate.h"
#include "Luma/Math/Math.h"
#include "Luma/Physics/BoxShape.h"
#include "Luma/Physics/PlaneShape.h"
#include "Luma/Rendering/Renderer2D.h"

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

    Ref<FPhysicsWorld> physicsWorld = Ref<FPhysicsWorld>::create();

    Ref<FPhysicsBody> floor = physicsWorld->createBody({EPhysicsBodyType::Static});
    Ref<FPlaneShape> floorShape = Ref<FPlaneShape>::create();
    floor->attachShape(floorShape);


    Ref<FPhysicsBody> cube = physicsWorld->createBody({EPhysicsBodyType::Dynamic, FVector3f(0.0f, 10.0f, 0.0f)});
    cube->setConstraints(EPhysicsConstraintsBits::PositionX|EPhysicsConstraintsBits::PositionZ);

    Ref<FBoxShape> boxShape = Ref<FBoxShape>::create(FVector3f(0.5f, 0.5f, 0.5f));
    cube->attachShape(boxShape);

    while (!window->shouldClose())
    {
        window->pollEvents();

        physicsWorld->step();

        if (FInput::getKeyDown(EKey::Space))
        {
            FVector3f velocity = std::sqrt(-2.0f * physicsWorld->getGravity().y * 5.0f);
            cube->setLinearVelocity(velocity);
        }

        FVector3f position = cube->getPosition();
        

        renderer->begin();
        renderer->drawText(strfmt("Position: ({:.3f}, {:.3f}, {:.3f})", position.x, position.y, position.z), {0, 0}, 20, FColor::Cyan);
        renderer->drawText(strfmt("Rotation: ({:.3f}, {:.3f}, {:.3f})", position.x, position.y, position.z), {0, 0}, 40, FColor::Cyan);
        renderer->drawText(strfmt("Mass: {}", cube->getMass()), {0, 60}, 20, FColor::Cyan);
        renderer->drawText(strfmt("Center of Mass: ({:.3f}, {:.3f}, {:.3f})", cube->getCenterOfMass().x, cube->getCenterOfMass().y, cube->getCenterOfMass().z), {0, 80}, 20, FColor::Cyan);
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
