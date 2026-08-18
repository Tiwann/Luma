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
    static constexpr DebugLevel getDebugLevel() { return DebugLevel::Debug; }
#elifdef LUMA_DEV
    static constexpr DebugLevel getDebugLevel() { return DebugLevel::Dev; }
#else
    static constexpr DebugLevel getDebugLevel() { return DebugLevel::Release; }
#endif
}