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
}