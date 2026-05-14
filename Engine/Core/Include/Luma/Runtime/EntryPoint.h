#pragma once

namespace Luma
{
#ifdef LUMA_CUSTOM_ENTRY_POINT
    extern int guardedMain(int argc, char** argv);
#else
    extern class IApplication* createApplication(int argc, char** argv);
#endif
}