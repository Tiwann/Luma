#include <Luma/Memory/Ref.h>
#include <Luma/Runtime/DesktopWindow.h>
#include <Luma/Runtime/Flags.h>
#include <Luma/Runtime/Path.h>
#include <Luma/Runtime/Time.h>
#include <Luma/Asset/StaticMesh.h>
#include <Luma/Rendering.h>

#include "Luma/Components/CameraComponent.h"
#include "Luma/Components/StaticMeshRenderer.h"
#include "Luma/Runtime/EntityHandle.h"

using namespace Luma;

static constexpr uint32_t WIDTH = 800;
static constexpr uint32_t HEIGHT = 600;

int main()
{
    constexpr auto flags = TFlags(EWindowCreateBits::Centered);
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
    renderDeviceDesc.vSync = false;

    Ref<IRenderDevice> renderDevice = createRenderDevice(renderDeviceDesc);
    LUMA_ASSERT(renderDevice, "Render device failed to create! Exiting application.");

    FString filepath = FPath::openFileDialog("Open a model file.", FPath::getDesktopDirectory(), FDialogFilters::ModelFilters, *window);
    if (filepath.isEmpty()) return 1;

    Ref<FStaticMesh> staticMesh = new FStaticMesh();
    if (!staticMesh->loadFromFile(filepath, renderDevice)) return 1;


    Ref<FScene> scene = new FScene();
    FEntityHandle meshEntity = scene->createEntity();
    FStaticMeshRenderer* renderer = meshEntity->addComponent<FStaticMeshRenderer>();
    renderer->setMesh(staticMesh);

    FEntityHandle cameraEntity = scene->createEntity();
    FCameraComponent* cameraComponent = cameraEntity->addComponent<FCameraComponent>();
    cameraComponent->setPosition(FVector3f(0.0f, 0.0f, -1.0f));
    cameraComponent->setDimensions(WIDTH, HEIGHT);
    window->resizedEvent.bindMember<FCameraComponent>(cameraComponent, &FCameraComponent::setDimensions);

    FSceneRendererDesc rendererDesc;
    rendererDesc.device = renderDevice;
    rendererDesc.width = WIDTH;
    rendererDesc.height = HEIGHT;

    Ref<FSceneRenderer> sceneRenderer = new FSceneRenderer();
    if (!sceneRenderer->initialize(rendererDesc)) return 1;
    window->resizedEvent.bindMember(sceneRenderer.get(), &FSceneRenderer::resize);
    sceneRenderer->setScene(scene);

    while (!window->shouldClose())
    {
        window->pollEvents();

        sceneRenderer->begin();

        sceneRenderer->end();

        if (renderDevice->beginFrame())
        {
            ICommandBuffer* cmdBuffer = renderDevice->getCommandBuffer();
            const ITextureView* swapchainTexture = renderDevice->getAcquiredSwapchainTextureView();
            sceneRenderer->render(cmdBuffer);

            /*FRenderPassAttachment colorAttachment;
            colorAttachment.type = ERenderPassAttachmentType::Color;
            colorAttachment.loadOp = ELoadOp::Clear;
            colorAttachment.storeOp = EStoreOp::Store;
            colorAttachment.clearValue.color = FColor::Black;
            colorAttachment.textureView = swapchainTexture;

            FRenderPassDesc renderPassDesc;
            renderPassDesc.renderArea = {0, 0, WIDTH, HEIGHT};
            renderPassDesc.colorAttachments.add(&colorAttachment);

            cmdBuffer->beginRenderPass(renderPassDesc);
            cmdBuffer->endRenderPass();
            */

            renderDevice->endFrame();
            renderDevice->present();
        }
    }

    renderDevice->waitIdle();
    return 0;
}
