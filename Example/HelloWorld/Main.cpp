#include <Luma/Runtime/DesktopWindow.h>
#include <Luma/Rendering/RenderDevice.h>
#include <Luma/Memory/Ref.h>
#include <Luma/Rendering/ShaderCompiler.h>
#include <Luma/Runtime/Flags.h>
#include <Luma/Rendering/Shader.h>


using namespace Luma;

int main()
{

    FWindowDesc windowDesc;
    windowDesc.title = "First window";
    windowDesc.width = 800;
    windowDesc.height = 600;
    windowDesc.flags = 0;
    Ref<FDesktopWindow> window = Ref(createWindow(windowDesc));
    assert(window, "Failed to create window! Exiting application.");

    FRenderDeviceDesc deviceDesc;
    deviceDesc.deviceType = ERenderDeviceType::Vulkan;
    deviceDesc.buffering = ESwapchainBuffering::TripleBuffering;
    deviceDesc.window = window.get();
    deviceDesc.vSync = false;

    Ref<IRenderDevice> renderDevice = Ref(createRenderDevice(deviceDesc));
    assert(renderDevice, "Render device failed to create! Exiting application.")

    FShaderDesc shaderDesc;
    shaderDesc.moduleName = "Fullscreen";
    shaderDesc.filepath = R"(D:\Dev\Luma\Engine\Shaders\Fullscreen.slang)";
    shaderDesc.stageFlags = EShaderStageBits::Vertex | EShaderStageBits::Fragment;

    Ref<IShader> shader = Ref(renderDevice->createShader(shaderDesc));

    while (!window->shouldClose())
    {
        window->pollEvents();

        if (renderDevice->beginFrame())
        {
            renderDevice->endFrame();
            renderDevice->present();
        }
    }
    return 0;
}
