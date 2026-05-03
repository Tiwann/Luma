#include <Luma/Rendering.h>
#include <Luma/Memory/Ref.h>
#include <Luma/Runtime/DesktopWindow.h>
#include <Luma/Runtime/Flags.h>
#include <Luma/Runtime/Path.h>
#include <Luma/Runtime/Time.h>
#include <Luma/Asset/Material.h>
#include <Luma/Containers/StringFormat.h>

using namespace Luma;

static constexpr uint32_t WIDTH = 800;
static constexpr uint32_t HEIGHT = 600;

extern bool g_Vsync;
extern ESwapchainBuffering g_Buffering;
extern ERenderDeviceType g_DeviceType;
bool parseArgs(int, const char**);


int main(int argc, const char** argv)
{
    if (!parseArgs(argc, argv))
        return 1;

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
    renderDeviceDesc.buffering = g_Buffering;
    renderDeviceDesc.window = window;
    renderDeviceDesc.vSync = g_Vsync;

    Ref<IRenderDevice> renderDevice = createRenderDevice(renderDeviceDesc);
    LUMA_ASSERT(renderDevice, "Render device failed to create! Exiting application.");
    window->resizedEvent.bind([&renderDevice](uint32_t, uint32_t)
    {
        ISwapchain* swapchain = renderDevice->getSwapchain();
        swapchain->invalidate();
    });

    FShaderDesc shaderDesc;
    shaderDesc.stageFlags = EShaderStageBits::Vertex | EShaderStageBits::Fragment;
    shaderDesc.moduleName = "HelloTexture";
    shaderDesc.filepath = FPath::getAssetPath("Shaders/HelloTexture.slang");

    Ref<IShader> shader = renderDevice->createShader(shaderDesc);
    LUMA_ASSERT(shader, "Failed to create shader! Exiting application.");

    FGraphicsPipelineDesc pipelineDesc;
    pipelineDesc.device = renderDevice;
    pipelineDesc.shaderProgram = shader;
    pipelineDesc.depthStencil.depthTestEnable = false;
    pipelineDesc.depthStencil.stencilTestEnable = false;
    pipelineDesc.depthStencil.depthWriteEnable = false;
    pipelineDesc.colorFormats[0] = EFormat::R8G8B8A8_SRGB;
    pipelineDesc.colorFormatCount = 1;

    Ref<IGraphicsPipeline> pipeline = renderDevice->createGraphicsPipeline(pipelineDesc);
    LUMA_ASSERT(pipeline, "Failed to create graphics pipeline! Exiting application.");

    FSamplerDesc samplerDesc;
    samplerDesc.minFilter = EFilter::Linear;
    samplerDesc.magFilter = EFilter::Linear;
    Ref<ISampler> sampler = renderDevice->createSampler(samplerDesc);
    LUMA_ASSERT(sampler, "Failed to create sampler! Exiting application.");

    Ref<ITexture> texture = TextureUtils::loadTexture(renderDevice, FPath::getAssetPath("Textures/minecraft-grass.png"));
    LUMA_ASSERT(texture, "Failed to load texture! Exiting application.");

    FMaterialDesc materialDesc;
    materialDesc.shader = shader;
    Ref<FMaterial> material = renderDevice->createMaterial(materialDesc);
    LUMA_ASSERT(material, "Failed to create material! Exiting application.");
    material->setCombinedTextureSampler("texture", sampler, texture);

    double lastTime = 0.0;
    while (!window->shouldClose())
    {
        double currentTime = FTime::getTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        static double timer = 0.0;
        timer += deltaTime;
        if (timer >= 1.0)
        {
            timer = 0.0;
            int32_t fps = 1.0 / deltaTime;
            window->setTitle(strfmt("Hello Texture | FPS: {}", fps));
        }

        window->pollEvents();

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
            cmdBuffer->bindMaterial(material);
            cmdBuffer->bindGraphicsPipeline(pipeline);
            cmdBuffer->setViewport({0.0f, 0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, 1.0f});
            cmdBuffer->setScissor({0, 0, WIDTH, HEIGHT});
            cmdBuffer->draw(6, 1, 0, 0);
            cmdBuffer->endRenderPass();

            renderDevice->endFrame();
            renderDevice->present();
        }
    }

    renderDevice->waitIdle();
    return 0;
}
