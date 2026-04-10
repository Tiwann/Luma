#include <iostream>
#include <Luma/Runtime/DesktopWindow.h>

using namespace luma;

int main()
{
    FWindowCreateInfo windowCreateInfo;
    windowCreateInfo.title = "First window";
    windowCreateInfo.width = 800;
    windowCreateInfo.height = 600;
    windowCreateInfo.flags = 0;

    FDesktopWindow window;
    if (!window.initialize(windowCreateInfo))
    {
        std::cout << "Failed to create window" << std::endl;
        return EXIT_FAILURE;
    }

    while (!window.shouldClose())
        window.pollEvents();

    window.destroy();
    return 0;
}
