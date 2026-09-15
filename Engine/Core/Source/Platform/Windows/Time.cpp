#include "Luma/Runtime/Time.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Luma
{
    double FTime::getTime()
    {
        static bool initialized = false;
        static LARGE_INTEGER frequency;
        static LARGE_INTEGER start;

        []()
        {
            if (!initialized)
            {
                QueryPerformanceFrequency(&frequency);
                QueryPerformanceCounter(&start);
                initialized = true;
            }
        }();


        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        return static_cast<double>(now.QuadPart - start.QuadPart) / static_cast<double>(frequency.QuadPart);
    }
}
