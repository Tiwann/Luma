#pragma once


namespace Luma
{
    enum class DebugLevel
    {
        Debug,
        Dev,
        Release,
    };

#ifdef LUMA_DEBUG
    static constexpr DebugLevel DEBUG_LEVEL = DebugLevel::Debug;
#elifdef LUMA_DEV
    static constexpr DebugLevel DEBUG_LEVEL = DebugLevel::Dev;
#else
    static constexpr DebugLevel DEBUG_LEVEL = DebugLevel::Release;
#endif
}