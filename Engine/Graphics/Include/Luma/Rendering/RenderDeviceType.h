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
#ifdef LUMA_BUILD_DEKO3D
        Deko3D,
#endif
    };


#ifdef LUMA_BUILD_VULKAN
    static constexpr bool hasVulkan = true;
#else
    static constexpr bool hasVulkan = false;
#endif


#ifdef LUMA_BUILD_D3D12
    static constexpr bool hasD3D12 = true;
#else
    static constexpr bool hasD3D12 = false;
#endif

#ifdef LUMA_BUILD_OPENGL
    static constexpr bool hasOpenGL = true;
#else
    static constexpr bool hasOpenGL = false;
#endif

#ifdef LUMA_BUILD_WEBGPU
    static constexpr bool hasWebGPU = true;
#else
    static constexpr bool hasWebGPU = false;
#endif

#ifdef LUMA_BUILD_DEKO3D
    static constexpr bool hasDeko3D = true;
#else
    static constexpr bool hasDeko3D = false;
#endif
}
