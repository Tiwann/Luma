#include "Luma/Runtime/Time.h"
#include <time.h>

namespace Luma
{
    static bool s_Initialized = false;
    static timespec s_Start;

    void FTime::initialize()
    {
        clock_gettime(CLOCK_MONOTONIC, &s_Start);
        s_Initialized = true;
    }

    double FTime::getTime()
    {
        if (!s_Initialized) initialize();
        timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        const double sec = static_cast<double>(now.tv_sec - s_Start.tv_sec);
        const double nsec = static_cast<double>(now.tv_nsec - s_Start.tv_nsec) / 1e9;
        return sec + nsec;
    }
}
