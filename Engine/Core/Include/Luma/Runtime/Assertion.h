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
#ifndef LUMA_ASSERT
#define LUMA_ASSERT(x, msg) \
    do { \
        if(!(x)) \
        { \
            std::cerr << msg << '\n' << "File: " << __FILE__ << '\n' << "Line: " << __LINE__ << '\n'; \
            LUMA_DBG_BREAK(); \
        } \
    } while(0)
#endif
#else
#ifndef LUMA_ASSERT
#define LUMA_ASSERT(x, msg) ((void)0)
#endif
#endif