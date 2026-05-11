#pragma once

namespace Luma
{
    enum class ERenderDeviceType
    {
        None,
#ifdef LUMA_BUILD_VULKAN
        Vulkan,
#endif
#ifdef LUMA_BUILD_D3D12
        D3D12,
#endif
#ifdef LUMA_BUILD_OPENGL
        OpenGL,
#endif
#ifdef LUMA_BUILD_WEBGPU
        WebGPU,
#endif
#ifdef LUMA_BUILD_NVN
        NVN,
#endif
#ifdef LUMA_BUILD_DEKO3D
        Deko3D,
#endif
#ifdef LUMA_BUILD_GNM
        GNM,
#endif
    };


#ifdef LUMA_BUILD_VULKAN
    static constexpr bool HasVulkan = true;
#else
    static constexpr bool HasVulkan = false;
#endif


#ifdef LUMA_BUILD_D3D12
    static constexpr bool HasD3D12 = true;
#else
    static constexpr bool HasD3D12 = false;
#endif

#ifdef LUMA_BUILD_OPENGL
    static constexpr bool HasOpenGL = true;
#else
    static constexpr bool HasOpenGL = false;
#endif

#ifdef LUMA_BUILD_WEBGPU
    static constexpr bool HasWebGPU = true;
#else
    static constexpr bool HasWebGPU = false;
#endif

#ifdef LUMA_BUILD_NVN
    static constexpr bool HasNVN = true;
#else
    static constexpr bool HasNVN = false;
#endif

#ifdef LUMA_BUILD_DEKO3D
    static constexpr bool HasDeko3D = true;
#else
    static constexpr bool HasDeko3D = false;
#endif

#ifdef LUMA_BUILD_GNM
    static constexpr bool HasGNM = true;
#else
    static constexpr bool HasGNM = false;
#endif
}
