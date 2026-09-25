#include <stb_image.h>
#include <Luma/Input/Immediate.h>
#include <Luma/Memory/Ref.h>
#include <Luma/Math/Math.h>
#include <Luma/Rendering/Device.h>
#include <Luma/Rendering/CommandBuffer.h>
#include <Luma/Rendering/Shader.h>
#include <Luma/Rendering/BindingGroup.h>
#include <Luma/Rendering/Sampler.h>
#include <Luma/Rendering/Texture.h>
#include <Luma/Rendering/RenderPipeline.h>
#include <Luma/Rendering/RenderPassDesc.h>
#include <Luma/Rendering/BufferUtils.h>
#include <Luma/Rendering/TextureUtils.h>
#include <Luma/Runtime/Path.h>
#include <Luma/Runtime/Time.h>
#include <Luma/Runtime/Window.h>

using namespace Luma;
using namespace Luma::RHI;

static constexpr uint32_t kWidth = 1280;
static constexpr uint32_t kHeight = 720;

int main()
{
    Ref<Window> window = createWindow("Hello Renderer2D", kWidth, kHeight, WindowOptions::Centered | WindowOptions::Resizable);
    Ref<Device> device = createDevice(window);
    device->setVSync(false);

    Camera camera;
    camera.setSize(kWidth, kHeight);
    camera.setProjectionMode(CameraProjectionMode::Perspective);
    camera.setFieldOfView(90);
    window->resizedEvent.bindMember(&camera, &Camera::setSize);

    int32_t width, height;
    const String path = Path::getAssetPath("HDRI/venice_sunset_8k.hdr");
    float* hdriPixels = stbi_loadf(*path, &width, &height, nullptr, STBI_rgb_alpha);
    const uint64_t hdriPixelsSize = width * height * 4 * sizeof(float);

    TextureDesc textureDesc = TextureDesc::texture2D(width, height, Format::R32G32B32A32_FLOAT);
    textureDesc.usageFlags |= TextureUsage::Color;
    Ref<Texture> hdriTexture = device->createTexture(textureDesc);
    TextureUtils::uploadTextureDataSync(device, hdriTexture, 0, 0, hdriPixels, hdriPixelsSize);
    stbi_image_free(hdriPixels);

    SamplerDesc samplerDesc;
    samplerDesc.minFilter = Filter::Linear;
    samplerDesc.magFilter = Filter::Linear;
    Ref<Sampler> sampler = device->createSampler(samplerDesc);

    const String vertexPath = Path::getAssetPath("Shaders/Skybox.slang.vert.spv");
    const String fragmentPath = Path::getAssetPath("Shaders/Skybox.slang.frag.spv");
    Ref<Shader> skyboxShader = device->createShader(vertexPath, fragmentPath);

    RenderPipelineDesc rpDesc;
    rpDesc.shaderProgram = skyboxShader;
    rpDesc.colorFormats[0] = Format::RGBA8_SRGB;
    rpDesc.colorTargetCount = 1;
    Ref<RenderPipeline> skyboxPipeline = device->createRenderPipeline(rpDesc);

    Ref<BindingGroup> bindingGroup = skyboxShader->createBindingGroup(0);
    bindingGroup->bindTextureWithSampler("hdriTexture", hdriTexture, sampler);

    float lastTime = 0.0f;
    float yaw = 0.0f, pitch = 0.0f;
    float sensitivity = 150.0f;

    while (!window->shouldClose())
    {
        window->pollEvents();
        const double currentTime = FTime::getTime();
        const float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        if (Input::getMouseButton(MouseButton::Right))
        {
            const FVector2<double> mouseDelta = Input::getMouseDelta();
            if (mouseDelta.magnitude() > 0.0f)
            {
                yaw += Math::Deg2Rad<float> * mouseDelta.x * sensitivity;
                pitch -= Math::Deg2Rad<float> * mouseDelta.y * sensitivity;
                pitch = Math::clamp(pitch, Math::Deg2Rad<float> * -80.0f, Math::Deg2Rad<float> * 80.0f);
                camera.setRotation(FQuatf::fromEulerAngles({pitch, yaw, 0.0f}));
            }
         }

        if (device->beginFrame())
        {
            CommandBuffer* cmdBuffer = device->getCommandBuffer();
            const FMatrix4f& viewProj = camera.getViewProjectionMatrix();

            struct PushConstants
            {
                FMatrix4f invViewProj;
                FVector3f camPos;
            } constants {viewProj.inverted(), camera.getPosition()};


            RenderPassTarget target;
            target.type = RenderPassTargetType::Color;
            target.loadOp = LoadOp::Clear;
            target.storeOp = StoreOp::Store;
            target.textureView = device->getAcquiredSwapchainTextureView();

            RenderPassDesc renderPassDesc;
            renderPassDesc.colorTargets.add(&target);
            renderPassDesc.renderArea = window->getBounds();

            cmdBuffer->beginRenderPass(renderPassDesc);
            cmdBuffer->pushConstant(skyboxShader, ShaderStage::Fragment, constants);
            cmdBuffer->bindBindingGroup(bindingGroup);
            cmdBuffer->bindRenderPipeline(skyboxPipeline);
            cmdBuffer->setViewport(Viewport::fromCamera(camera));
            cmdBuffer->setScissor(Scissor::fromCamera(camera));
            cmdBuffer->draw(6, 1, 0, 0);
            cmdBuffer->endRenderPass();

            device->endFrame();
            device->present();
        }
    }

    device->waitIdle();
    return 0;
}
