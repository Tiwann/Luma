#pragma once
#include "Luma/Runtime/Format.h"
#include "Luma/Rendering/QueueType.h"
#include "Luma/Rendering/BufferUsage.h"
#include "Luma/Rendering/Viewport.h"

#include <deko3d.h>
namespace Luma
{
    template<typename DekoType, typename NovaType>
    DekoType convert(const NovaType& value);

    template<>
    inline DkImageFormat convert(const EFormat& format)
    {
        switch (format)
        {
        case EFormat::None: return DkImageFormat_None;
        case EFormat::R8_UNORM: return DkImageFormat_R8_Unorm;
        case EFormat::R8_SNORM: return DkImageFormat_R8_Snorm;
        case EFormat::R16_USHORT: return DkImageFormat_R16_Uint;
        case EFormat::R16_SHORT: return DkImageFormat_R16_Sint;
        case EFormat::R32_FLOAT: return DkImageFormat_R32_Float;
        case EFormat::R32_UINT: return DkImageFormat_R32_Uint;
        case EFormat::R32_SINT: return DkImageFormat_R32_Sint;
        case EFormat::R8G8_UNORM: return DkImageFormat_RG8_Unorm;
        case EFormat::R8G8_SNORM: return DkImageFormat_RG8_Snorm;
        case EFormat::R16G16_USHORT: return DkImageFormat_RG16_Uint;
        case EFormat::R16G16_SHORT: return DkImageFormat_RG16_Sint;
        case EFormat::R32G32_UINT: return DkImageFormat_RG32_Uint;
        case EFormat::R32G32_SINT: return DkImageFormat_RG32_Sint;
        case EFormat::R32G32_FLOAT: return DkImageFormat_RG32_Float;
        case EFormat::R8G8B8_UNORM: return DkImageFormat_RGBX8_Unorm;
        case EFormat::R8G8B8_SNORM: return DkImageFormat_RGBX8_Snorm;
        case EFormat::R16G16B16_USHORT: return DkImageFormat_RGBX16_Uint;
        case EFormat::R16G16B16_SHORT: return DkImageFormat_RGBX16_Sint;
        case EFormat::R32G32B32_UINT: return DkImageFormat_RGBX32_Uint;
        case EFormat::R32G32B32_SINT: return DkImageFormat_RGBX32_Sint;
        case EFormat::R32G32B32_FLOAT: return DkImageFormat_RGBX32_Float;
        case EFormat::R8G8B8A8_UNORM: return DkImageFormat_RGBA8_Unorm;
        case EFormat::R8G8B8A8_SNORM: return DkImageFormat_RGBA8_Snorm;
        case EFormat::R8G8B8A8_SRGB: return DkImageFormat_RGBA8_Unorm_sRGB;
        case EFormat::R16G16B16A16_USHORT: return DkImageFormat_RGBA16_Uint;
        case EFormat::R16G16B16A16_SHORT: return DkImageFormat_RGBA16_Sint;
        case EFormat::R32G32B32A32_UINT: return DkImageFormat_RGBA32_Uint;
        case EFormat::R32G32B32A32_SINT: return DkImageFormat_RGBA32_Sint;
        case EFormat::R32G32B32A32_FLOAT: return DkImageFormat_RGBA32_Float;
        case EFormat::R64G64B64A64_FLOAT:
            break;
        case EFormat::D32_FLOAT: return DkImageFormat_ZF32;
        case EFormat::D32_FLOAT_S8_UINT: return DkImageFormat_ZF32_X24S8;
        case EFormat::D24_UNORM_S8_UINT: return DkImageFormat_Z24S8;
        case EFormat::ASTC_4X4_UNORM: return DkImageFormat_RGBA_ASTC_4x4;
        case EFormat::ASTC_4X4_FLOAT: return DkImageFormat_RGBA_ASTC_4x4;
        case EFormat::ASTC_4X4_SRGB: return DkImageFormat_RGBA_ASTC_4x4_sRGB;
        case EFormat::ASTC_5X4_UNORM: return DkImageFormat_RGBA_ASTC_5x4;
        case EFormat::ASTC_5X4_FLOAT: return DkImageFormat_RGBA_ASTC_5x4;
        case EFormat::ASTC_5X4_SRGB: return DkImageFormat_RGBA_ASTC_5x4_sRGB;
        case EFormat::ASTC_5X5_UNORM: return DkImageFormat_RGBA_ASTC_5x5;
        case EFormat::ASTC_5X5_FLOAT: return DkImageFormat_RGBA_ASTC_5x5;
        case EFormat::ASTC_5X5_SRGB: return DkImageFormat_RGBA_ASTC_5x5_sRGB;
        case EFormat::ASTC_6X5_UNORM: return DkImageFormat_RGBA_ASTC_6x5;
        case EFormat::ASTC_6X5_FLOAT: return DkImageFormat_RGBA_ASTC_6x5;
        case EFormat::ASTC_6X5_SRGB: return DkImageFormat_RGBA_ASTC_6x5_sRGB;
        case EFormat::ASTC_6X6_UNORM: return DkImageFormat_RGBA_ASTC_6x6;
        case EFormat::ASTC_6X6_FLOAT: return DkImageFormat_RGBA_ASTC_6x6;
        case EFormat::ASTC_6X6_SRGB: return DkImageFormat_RGBA_ASTC_6x6_sRGB;
        case EFormat::ASTC_8X5_UNORM: return DkImageFormat_RGBA_ASTC_8x5;
        case EFormat::ASTC_8X5_FLOAT: return DkImageFormat_RGBA_ASTC_8x5;
        case EFormat::ASTC_8X5_SRGB: return DkImageFormat_RGBA_ASTC_8x5_sRGB;
        case EFormat::ASTC_8X6_UNORM: return DkImageFormat_RGBA_ASTC_8x6;
        case EFormat::ASTC_8X6_FLOAT: return DkImageFormat_RGBA_ASTC_8x6;
        case EFormat::ASTC_8X6_SRGB: return DkImageFormat_RGBA_ASTC_8x6_sRGB;
        case EFormat::ASTC_8X8_UNORM: return DkImageFormat_RGBA_ASTC_8x8;
        case EFormat::ASTC_8X8_FLOAT: return DkImageFormat_RGBA_ASTC_8x8;
        case EFormat::ASTC_8X8_SRGB: return DkImageFormat_RGBA_ASTC_8x8_sRGB;
        case EFormat::ASTC_10X5_UNORM: return DkImageFormat_RGBA_ASTC_10x5;
        case EFormat::ASTC_10X5_FLOAT: return DkImageFormat_RGBA_ASTC_10x5;
        case EFormat::ASTC_10X5_SRGB: return DkImageFormat_RGBA_ASTC_10x5_sRGB;
        case EFormat::ASTC_10X6_UNORM: return DkImageFormat_RGBA_ASTC_10x6;
        case EFormat::ASTC_10X6_FLOAT: return DkImageFormat_RGBA_ASTC_10x6;
        case EFormat::ASTC_10X6_SRGB: return DkImageFormat_RGBA_ASTC_10x6_sRGB;
        case EFormat::ASTC_10X8_UNORM: return DkImageFormat_RGBA_ASTC_10x8;
        case EFormat::ASTC_10X8_FLOAT: return DkImageFormat_RGBA_ASTC_10x8;
        case EFormat::ASTC_10X8_SRGB: return DkImageFormat_RGBA_ASTC_10x8_sRGB;
        case EFormat::ASTC_10X10_UNORM: return DkImageFormat_RGBA_ASTC_10x10;
        case EFormat::ASTC_10X10_FLOAT: return DkImageFormat_RGBA_ASTC_10x10;
        case EFormat::ASTC_10X10_SRGB: return DkImageFormat_RGBA_ASTC_10x10_sRGB;
        case EFormat::ASTC_12X10_UNORM: return DkImageFormat_RGBA_ASTC_12x10;
        case EFormat::ASTC_12X10_FLOAT: return DkImageFormat_RGBA_ASTC_12x10;
        case EFormat::ASTC_12X10_SRGB: return DkImageFormat_RGBA_ASTC_12x10_sRGB;
        case EFormat::ASTC_12X12_UNORM: return DkImageFormat_RGBA_ASTC_12x12;
        case EFormat::ASTC_12X12_FLOAT: return DkImageFormat_RGBA_ASTC_12x12;
        case EFormat::ASTC_12X12_SRGB: return DkImageFormat_RGBA_ASTC_12x12_sRGB;
        case EFormat::BC1_UNORM: return DkImageFormat_RGB_BC1;
        case EFormat::BC1_SRGB: return DkImageFormat_RGB_BC1_sRGB;
        case EFormat::BC2_UNORM: return DkImageFormat_RGBA_BC2;
        case EFormat::BC2_SRGB: return DkImageFormat_RGBA_BC2_sRGB;
        case EFormat::BC3_UNORM: return DkImageFormat_RGBA_BC3;
        case EFormat::BC3_SRGB: return DkImageFormat_RGBA_BC3_sRGB;
        case EFormat::BC4_UNORM: return DkImageFormat_R_BC4_Unorm;
        case EFormat::BC4_SNORM: return DkImageFormat_R_BC4_Snorm;
        case EFormat::BC5_UNORM: return DkImageFormat_RG_BC5_Unorm;
        case EFormat::BC5_SNORM: return DkImageFormat_RG_BC5_Snorm;
        case EFormat::BC7_UNORM: return DkImageFormat_RGBA_BC7_Unorm;
        case EFormat::BC7_SRGB: return DkImageFormat_RGBA_BC7_Unorm_sRGB;
        }
        return DkImageFormat_None;
    }

    template<>
    inline uint32_t convert(const EQueueType& queue)
    {
        switch (queue)
        {
        case EQueueType::None: return 0;
        case EQueueType::Render: return DkQueueFlags_Graphics;
        case EQueueType::Compute: return DkQueueFlags_Compute;
        case EQueueType::Copy: return DkQueueFlags_Graphics;
        }
        return 0;
    }

    template<>
    inline uint32_t convert(const EBufferUsage& usage)
    {
        switch (usage)
        {
        case EBufferUsage::None: return 0;
        case EBufferUsage::VertexBuffer: return DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached;
        case EBufferUsage::IndexBuffer: return DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached;
        case EBufferUsage::UniformBuffer: return DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached;
        case EBufferUsage::StorageBuffer: return DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached;
        case EBufferUsage::StagingBuffer: return DkMemBlockFlags_CpuCached | DkMemBlockFlags_GpuUncached;
        case EBufferUsage::IndirectBuffer: return DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached;
        }
        return 0;
    }

    template<>
    inline DkViewport convert(const FViewport& viewport)
    {
        return DkViewport
        (
            viewport.x,
            viewport.y,
            viewport.width,
            viewport.height,
            viewport.minDepth,
            viewport.maxDepth
        );
    }

    template<>
    inline DkScissor convert(const FScissor& viewport)
    {
        return DkScissor
        (
            viewport.x,
            viewport.y,
            viewport.width,
            viewport.height
        );
    }
}
