#include "Luma/Runtime/Window.h"
#include "Luma/Math/Vector2.h"

#if defined(LUMA_PLATFORM_WINDOWS) || defined(LUMA_PLATFORM_LINUX) || defined(LUMA_PLATFORM_MACOS)
#include "Luma/Runtime/DesktopWindow.h"
#endif

namespace Luma
{
    FVector2u IWindow::getSize() const
    {
        return FVector2u{getWidth(), getHeight()};
    }

    IWindow* createWindow(const FWindowDesc& windowDesc)
    {
        IWindow* window = nullptr;
#if defined(LUMA_PLATFORM_WINDOWS) || defined(LUMA_PLATFORM_LINUX) || defined(LUMA_PLATFORM_MACOS)
        window = new FDesktopWindow();
#elif defined(LUMA_PLATFORM_SWITCH)
        (void)window;
        return nullptr;
#elif defined LUMA_PLATFORM_PS5
        (void)window;
        return nullptr;
#endif
        if (!window->initialize(windowDesc))
        {
            delete window;
            return nullptr;
        }
        return window;
    }

    IWindow* createWindow(const FString& title, uint32_t width, uint32_t height, FWindowCreateFlags flags, EGpuDeviceType deviceType)
    {
        return createWindow(FWindowDesc{title, width, height, flags, deviceType});
    }
}
