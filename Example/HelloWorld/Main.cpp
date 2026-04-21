#include <Luma/Runtime/DesktopWindow.h>
#include <Luma/Rendering/RenderDevice.h>
#include <Luma/Memory/Ref.h>
#include <Luma/Runtime/Assertion.h>
#include <iostream>

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
