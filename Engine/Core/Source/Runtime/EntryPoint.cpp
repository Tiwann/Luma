#include "Luma/Runtime/EntryPoint.h"

#ifndef LUMA_CUSTOM_ENTRY_POINT
#include "Luma/Runtime/Application.h"
namespace Luma
{
    int guardedMain(const int argc, char** argv)
    {
        IApplication* application = createApplication(argc, argv);
        application->run();
        delete application;
        return 0;
    }
}
#endif

#if defined(LUMA_PLATFORM_WINDOWS) && defined(LUMA_RELEASE)
#include <Windows.h>
INT WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int)
{
    return Luma::guardedMain(__argc, __argv);
}
#else
int main(const int argc, char** argv)
{
    return Luma::guardedMain(argc, argv);
}
#endif
