#include "Luma/Runtime/Time.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Luma
{
    static bool s_Initialized = false;
    static LARGE_INTEGER s_Frequency;
    static LARGE_INTEGER s_Start;

    void FTime::initialize()
    {
        QueryPerformanceFrequency(&s_Frequency);
        QueryPerformanceCounter(&s_Start);
        s_Initialized = true;
    }

    double FTime::getTime()
    {
        if (!s_Initialized) initialize();
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        return static_cast<double>(now.QuadPart - s_Start.QuadPart) / static_cast<double>(s_Frequency.QuadPart);
    }
}
