#include <thread>
#include <Luma/Runtime/DesktopWindow.h>
#include <Luma/Rendering/RenderDevice.h>
#include <Luma/Memory/Ref.h>
#include <Luma/Rendering/ShaderCompiler.h>
#include <Luma/Runtime/Flags.h>
#include <Luma/Rendering/Shader.h>


using namespace Luma;

static Ref<FDesktopWindow> gWindow = nullptr;
static Ref<IRenderDevice> gRenderDevice = nullptr;

int main()
{

    FWindowDesc windowDesc;
    windowDesc.title = "First window";
    windowDesc.width = 800;
    windowDesc.height = 600;
    windowDesc.flags = 0;
    gWindow = Ref(createWindow(windowDesc));
    assert(gWindow, "Failed to create window! Exiting application.");

    FRenderDeviceDesc deviceDesc;
    deviceDesc.deviceType = ERenderDeviceType::Vulkan;
    deviceDesc.buffering = ESwapchainBuffering::TripleBuffering;
    deviceDesc.window = gWindow.get();
    deviceDesc.vSync = false;

    gRenderDevice = Ref(createRenderDevice(deviceDesc));
    assert(gRenderDevice, "Render device failed to create! Exiting application.")

    FShaderDesc shaderDesc;
    shaderDesc.moduleName = "Fullscreen";
    shaderDesc.filepath = R"(D:\Dev\Luma\Engine\Shaders\Fullscreen.slang)";
    shaderDesc.stageFlags = EShaderStageBits::Vertex | EShaderStageBits::Fragment;

    //Ref<IShader> shader = Ref(renderDevice->createShader(shaderDesc));
    while (!gWindow->shouldClose())
    {
        gWindow->pollEvents();

        if (gRenderDevice->beginFrame())
        {
            gRenderDevice->endFrame();
            gRenderDevice->present();
        }
    }
    return 0;
}
