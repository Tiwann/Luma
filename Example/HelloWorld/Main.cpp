#include <Luma/Runtime/DesktopWindow.h>
#include <Luma/Rendering/RenderDevice.h>
#include <Luma/Memory/Ref.h>
#include <Luma/Runtime/Assertion.h>
#include <Luma/Rendering/ShaderCompiler.h>
#include <iostream>
#include <slang/slang.h>
#include <slang/slang-com-ptr.h>

using namespace Luma;

int main()
{
#if 0
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

    while (!window->shouldClose())
    {
        window->pollEvents();

        if (renderDevice->beginFrame())
        {
            renderDevice->endFrame();
            renderDevice->present();
        }
    }
#else
    Slang::ComPtr<slang::IGlobalSession> globalSession = nullptr;
    if (SLANG_FAILED(slang::createGlobalSession(globalSession.writeRef())))
    {
        std::cout << "Failed to create global session" << std::endl;
        return 1;
    }

    FShaderCompileRequest request;
    request.setLanguage(EShadingLanguage::Slang);
    request.setCompileTarget(EShaderCompileTarget::SPIRV);
    request.setModuleInfo("Fullscreen", R"(D:\Dev\Luma\Engine\Shaders\Fullscreen.slang)");
    request.addEntryPoint("vert", EShaderStageBits::Vertex);
    request.addEntryPoint("frag", EShaderStageBits::Fragment);

    FShaderCompiler compiler(globalSession.get());
    compiler.addCompileRequest(request);

    const auto results = compiler.compile();
    if (results.isEmpty()) return 1;
#endif
    return 0;
}
