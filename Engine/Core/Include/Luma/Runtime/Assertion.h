#pragma once
#include <string>
#include <iostream>


#ifndef LUMA_DBG_BREAK
#ifdef LUMA_PLATFORM_WINDOWS
#define LUMA_DBG_BREAK() __debugbreak()
#else
#define LUMA_DBG_BREAK __builtin_trap()
#endif
#endif

#if defined(LUMA_DEBUG) ||defined(LUMA_DEV)
#ifndef assert
#define assert(x, msg) \
    if(!(x)) \
    { \
        std::cerr << msg << '\n' << "File: " << __FILE__ << '\n' << "Line: " << __LINE__ << '\n'; \
        LUMA_DBG_BREAK(); \
    }
#endif
#else
#ifndef assert
#define assert(x, msg) ((void)0)
#endif
#endif