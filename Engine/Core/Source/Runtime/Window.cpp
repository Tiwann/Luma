#include "Luma/Runtime/Window.h"

#if defined(LUMA_PLATFORM_WINDOWS) || defined(LUMA_PLATFORM_LINUX) || defined(LUMA_PLATFORM_MACOS)
#include "Luma/Runtime/DesktopWindow.h"
#endif

namespace Luma
{
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
}