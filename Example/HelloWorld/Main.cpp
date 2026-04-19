#include <iostream>
#include <Luma/Runtime/DesktopWindow.h>
#include <Luma/Rendering/RenderDevice.h>
#include <Luma/Memory/SharedRef.h>
#include <Luma/Runtime/Assertion.h>

using namespace Luma;

int main()
{
    FWindowDesc windowDesc;
    windowDesc.title = "First window";
    windowDesc.width = 800;
    windowDesc.height = 600;
    windowDesc.flags = 0;

    FDesktopWindow window;
    if (!window.initialize(windowDesc))
    {
        std::wcout << L"Failed to create window\n";
        return EXIT_FAILURE;
    }

    FRenderDeviceDesc deviceDesc;
    deviceDesc.deviceType = ERenderDeviceType::Vulkan;
    deviceDesc.buffering = ESwapchainBuffering::TripleBuffering;
    deviceDesc.window = &window;
    deviceDesc.vSync = false;

    TSharedRef<IRenderDevice> renderDevice = makeShared<IRenderDevice>(createRenderDevice(deviceDesc));
    assert(renderDevice, "Render device failed to create! Exiting application.")

    while (!window.shouldClose())
    {
        window.pollEvents();

        if (renderDevice->beginFrame())
        {
            renderDevice->endFrame();
            renderDevice->present();
        }
    }

    window.destroy();
    return 0;
}
