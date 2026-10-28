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
    static constexpr DebugLevel kDebugLevel = DebugLevel::Debug;
    static constexpr StringView kDebugLevelName = "Debug";
#elifdef LUMA_DEV
    static constexpr DebugLevel kDebugLevel = DebugLevel::Dev;
    static constexpr StringView kDebugLevelName = "Dev";
#else
    static constexpr DebugLevel kDebugLevel = DebugLevel::Release;
    static constexpr StringView kDebugLevelName = "Release";
#endif


}