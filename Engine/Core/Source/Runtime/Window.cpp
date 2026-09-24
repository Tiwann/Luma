#include "Luma/Runtime/Window.h"
#include "Luma/Math/Vector2.h"

#if defined(LUMA_PLATFORM_WINDOWS) || defined(LUMA_PLATFORM_LINUX) || defined(LUMA_PLATFORM_MACOS)
#include "Luma/Runtime/DesktopWindow.h"
#endif

namespace Luma
{
    FVector2u Window::getSize() const
    {
        return FVector2u{getWidth(), getHeight()};
    }

    Window* createWindow(const WindowDesc& windowDesc)
    {
        Window* window = nullptr;
#if defined(LUMA_PLATFORM_WINDOWS) || defined(LUMA_PLATFORM_LINUX) || defined(LUMA_PLATFORM_MACOS)
        window = new DesktopWindow();
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

    Window* createWindow(const String& title, uint32_t width, uint32_t height, WindowOptionsFlags options, DeviceType deviceType)
    {
        return createWindow(WindowDesc{title, width, height, options, deviceType});
    }
}
