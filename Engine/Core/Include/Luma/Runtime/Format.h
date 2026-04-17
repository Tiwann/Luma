#pragma once

namespace Luma
{
    enum class EFormat
    {
        None,
        R8_UNORM,
        R8_SNORM,
        R16_USHORT,
        R16_SHORT,
        R32_FLOAT,
        R32_UINT,
        R32_SINT,
        R8G8_UNORM,
        R8G8_SNORM,
        R16G16_USHORT,
        R16G16_SHORT,
        R32G32_UINT,
        R32G32_SINT,
        R32G32_FLOAT,
        R8G8B8_UNORM,
        R8G8B8_SNORM,
        R16G16B16_USHORT,
        R16G16B16_SHORT,
        R32G32B32_UINT,
        R32G32B32_SINT,
        R32G32B32_FLOAT,
        R8G8B8A8_UNORM,
        R8G8B8A8_SNORM,
        R8G8B8A8_SRGB,
        R16G16B16A16_USHORT,
        R16G16B16A16_SHORT,
        R32G32B32A32_UINT,
        R32G32B32A32_SINT,
        R32G32B32A32_FLOAT,
        R64G64B64A64_FLOAT,
        D32_FLOAT,
        D32_FLOAT_S8_UINT,
        D24_UNORM_S8_UINT,

        // Astc formats
        ASTC_4X4_UNORM,
        ASTC_4X4_FLOAT,
        ASTC_4X4_SRGB,
        ASTC_5X4_UNORM,
        ASTC_5X4_FLOAT,
        ASTC_5X4_SRGB,
        ASTC_5X5_UNORM,
        ASTC_5X5_FLOAT,
        ASTC_5X5_SRGB,
        ASTC_6X5_UNORM,
        ASTC_6X5_FLOAT,
        ASTC_6X5_SRGB,
        ASTC_6X6_UNORM,
        ASTC_6X6_FLOAT,
        ASTC_6X6_SRGB,
        ASTC_8X5_UNORM,
        ASTC_8X5_FLOAT,
        ASTC_8X5_SRGB,
        ASTC_8X6_UNORM,
        ASTC_8X6_FLOAT,
        ASTC_8X6_SRGB,
        ASTC_8X8_UNORM,
        ASTC_8X8_FLOAT,
        ASTC_8X8_SRGB,
        ASTC_10X5_UNORM,
        ASTC_10X5_FLOAT,
        ASTC_10X5_SRGB,
        ASTC_10X6_UNORM,
        ASTC_10X6_FLOAT,
        ASTC_10X6_SRGB,
        ASTC_10X8_UNORM,
        ASTC_10X8_FLOAT,
        ASTC_10X8_SRGB,
        ASTC_10X10_UNORM,
        ASTC_10X10_FLOAT,
        ASTC_10X10_SRGB,
        ASTC_12X10_UNORM,
        ASTC_12X10_FLOAT,
        ASTC_12X10_SRGB,
        ASTC_12X12_UNORM,
        ASTC_12X12_FLOAT,
        ASTC_12X12_SRGB,

        // Compressed formats
        BC1_UNORM,
        BC1_SRGB,
        BC2_UNORM,
        BC2_SRGB,
        BC3_UNORM,
        BC3_SRGB,
        BC4_UNORM,
        BC4_SNORM,
        BC5_UNORM,
        BC5_SNORM,
        BC7_UNORM,
        BC7_SRGB,

        // Aliases
        Float = R32_FLOAT,
        Vector2 = R32G32_FLOAT,
        Vector3 = R32G32B32_FLOAT,
        Vector4 = R32G32B32A32_FLOAT,
        Uint32 = R32_UINT,
        Uint16 = R16_USHORT,
        Uint8 = R8_UNORM,


        Float2 = Vector2,
        Float3 = Vector3,
        Float4 = Vector4,
    };

    static size_t getFormatSize(const EFormat format)
    {
        switch (format)
        {
        case EFormat::None:                 return 0ULL;
        case EFormat::R8G8B8A8_UNORM:       return 4ULL;
        case EFormat::R16G16B16A16_USHORT:  return 4ULL * 2ULL;
        case EFormat::R32G32B32A32_FLOAT:   return 4ULL * 4ULL;
        case EFormat::R8_UNORM:             return 1ULL;
        case EFormat::R8_SNORM:             return 1ULL * 1ULL;
        case EFormat::R16_USHORT:           return 2ULL * 1ULL;
        case EFormat::R16_SHORT:            return 2ULL * 1ULL;
        case EFormat::R32_FLOAT:            return 4ULL * 1ULL;
        case EFormat::R32_UINT:             return 4ULL * 1ULL;
        case EFormat::R32_SINT:             return 4ULL * 1ULL;
        case EFormat::R8G8_UNORM:           return 2ULL * 1ULL;
        case EFormat::R8G8_SNORM:           return 2ULL * 1ULL;
        case EFormat::R16G16_USHORT:        return 2ULL * 2ULL;
        case EFormat::R16G16_SHORT:         return 2ULL * 2ULL;
        case EFormat::R32G32_UINT:          return 2ULL * 4ULL;
        case EFormat::R32G32_SINT:          return 2ULL * 4ULL;
        case EFormat::R32G32_FLOAT:         return 2ULL * 4ULL;
        case EFormat::R8G8B8_UNORM:         return 3ULL * 1ULL;
        case EFormat::R8G8B8_SNORM:         return 3ULL * 1ULL;
        case EFormat::R16G16B16_USHORT:     return 3ULL * 2ULL;
        case EFormat::R16G16B16_SHORT:      return 3ULL * 2ULL;
        case EFormat::R32G32B32_UINT:       return 3ULL * 4ULL;
        case EFormat::R32G32B32_SINT:       return 3ULL * 4ULL;
        case EFormat::R32G32B32_FLOAT:      return 3ULL * 4ULL;
        case EFormat::R8G8B8A8_SNORM:       return 4ULL * 1ULL;
        case EFormat::R16G16B16A16_SHORT:   return 4ULL * 2ULL;
        case EFormat::R32G32B32A32_UINT:    return 4ULL * 4ULL;
        case EFormat::R32G32B32A32_SINT:    return 4ULL * 4ULL;
        case EFormat::R64G64B64A64_FLOAT: return 4ULL * 8ULL;
        case EFormat::R8G8B8A8_SRGB:        return 4ULL * 1ULL;
        case EFormat::D32_FLOAT:            return 4ULL;
        case EFormat::D32_FLOAT_S8_UINT:    return 5ULL;
        case EFormat::D24_UNORM_S8_UINT:     return 4ULL;
        case EFormat::ASTC_4X4_UNORM:
            break;
        case EFormat::ASTC_6X6_UNORM:
            break;
        case EFormat::ASTC_8X8_UNORM:
            break;
        case EFormat::ASTC_12X12_UNORM:
            break;
        case EFormat::ASTC_4X4_SRGB:
            break;
        case EFormat::ASTC_6X6_SRGB:
            break;
        case EFormat::ASTC_8X8_SRGB:
            break;
        case EFormat::ASTC_12X12_SRGB:
            break;
        default:                           return 0ULL;
        }
        return size_t(0);
    }

    static size_t getFormatBytesPerChannel(const EFormat format)
    {
        switch (format)
        {
        case EFormat::None:  return 0;
        case EFormat::R8_UNORM: return 1ULL;
        case EFormat::R8_SNORM: return 1ULL;
        case EFormat::R16_USHORT: return 2ULL;
        case EFormat::R16_SHORT: return 2ULL;
        case EFormat::R32_FLOAT: return 4ULL;
        case EFormat::R32_UINT: return 4ULL;
        case EFormat::R32_SINT: return 4ULL;
        case EFormat::R8G8_UNORM: return 1ULL;
        case EFormat::R8G8_SNORM: return 1ULL;
        case EFormat::R16G16_USHORT: return 2ULL;
        case EFormat::R16G16_SHORT: return 2ULL;
        case EFormat::R32G32_UINT: return 4ULL;
        case EFormat::R32G32_SINT: return 4ULL;
        case EFormat::R32G32_FLOAT: return 4ULL;
        case EFormat::R8G8B8_UNORM: return 1ULL;
        case EFormat::R8G8B8_SNORM: return 1ULL;
        case EFormat::R16G16B16_USHORT: return 2ULL;
        case EFormat::R16G16B16_SHORT: return 2ULL;
        case EFormat::R32G32B32_UINT: return 4ULL;
        case EFormat::R32G32B32_SINT: return 4ULL;
        case EFormat::R32G32B32_FLOAT: return 4ULL;
        case EFormat::R8G8B8A8_UNORM: return 1ULL;
        case EFormat::R8G8B8A8_SNORM: return 1ULL;
        case EFormat::R16G16B16A16_USHORT: return 2ULL;
        case EFormat::R16G16B16A16_SHORT: return 2ULL;
        case EFormat::R32G32B32A32_UINT: return 4ULL;
        case EFormat::R32G32B32A32_SINT: return 4ULL;
        case EFormat::R32G32B32A32_FLOAT: return 4ULL;
        case EFormat::R64G64B64A64_FLOAT: return 8ULL;
        case EFormat::R8G8B8A8_SRGB: return 1ULL;
        case EFormat::D32_FLOAT: return 4ULL;
        case EFormat::D32_FLOAT_S8_UINT: return 4ULL;
        case EFormat::D24_UNORM_S8_UINT: return 3ULL;
        default: return 0;
        }
    }

    static size_t getFormatComponentCount(const EFormat format)
    {
        switch (format)
        {
        case EFormat::None: return 0ULL;
        case EFormat::R8_UNORM: return 1ULL;
        case EFormat::R8_SNORM: return 1ULL;
        case EFormat::R16_USHORT: return 1ULL;
        case EFormat::R16_SHORT: return 1ULL;
        case EFormat::R32_FLOAT: return 1ULL;
        case EFormat::R32_UINT: return 1ULL;
        case EFormat::R32_SINT: return 1ULL;
        case EFormat::R8G8_UNORM: return 2ULL;
        case EFormat::R8G8_SNORM: return 2ULL;
        case EFormat::R16G16_USHORT: return 2ULL;
        case EFormat::R16G16_SHORT: return 2ULL;
        case EFormat::R32G32_UINT: return 2ULL;
        case EFormat::R32G32_SINT: return 2ULL;
        case EFormat::R32G32_FLOAT: return 2ULL;
        case EFormat::R8G8B8_UNORM: return 3ULL;
        case EFormat::R8G8B8_SNORM: return 3ULL;
        case EFormat::R16G16B16_USHORT: return 3ULL;
        case EFormat::R16G16B16_SHORT: return 3ULL;
        case EFormat::R32G32B32_UINT: return 3ULL;
        case EFormat::R32G32B32_SINT: return 3ULL;
        case EFormat::R32G32B32_FLOAT: return 3ULL;
        case EFormat::R8G8B8A8_UNORM: return 4ULL;
        case EFormat::R8G8B8A8_SNORM: return 4ULL;
        case EFormat::R16G16B16A16_USHORT: return 4ULL;
        case EFormat::R16G16B16A16_SHORT: return 4ULL;
        case EFormat::R32G32B32A32_UINT: return 4ULL;
        case EFormat::R32G32B32A32_SINT: return 4ULL;
        case EFormat::R32G32B32A32_FLOAT: return 4ULL;
            case EFormat::R64G64B64A64_FLOAT: return 4ULL;
        case EFormat::R8G8B8A8_SRGB: return 4ULL;
        case EFormat::D32_FLOAT: return 1ULL;
        case EFormat::D32_FLOAT_S8_UINT: return 2ULL;
        case EFormat::D24_UNORM_S8_UINT: return 2ULL;
        default: throw;
        }
    }

    static bool isDepthFormat(const EFormat format)
    {
        switch (format)
        {
        case EFormat::None: return false;
        case EFormat::R8_UNORM:
        case EFormat::R8_SNORM:
        case EFormat::R16_USHORT:
        case EFormat::R16_SHORT:
        case EFormat::R32_FLOAT:
        case EFormat::R32_UINT:
        case EFormat::R32_SINT:
        case EFormat::R8G8_UNORM:
        case EFormat::R8G8_SNORM:
        case EFormat::R16G16_USHORT:
        case EFormat::R16G16_SHORT:
        case EFormat::R32G32_UINT:
        case EFormat::R32G32_SINT:
        case EFormat::R32G32_FLOAT:
        case EFormat::R8G8B8_UNORM:
        case EFormat::R8G8B8_SNORM:
        case EFormat::R16G16B16_USHORT:
        case EFormat::R16G16B16_SHORT:
        case EFormat::R32G32B32_UINT:
        case EFormat::R32G32B32_SINT:
        case EFormat::R32G32B32_FLOAT:
        case EFormat::R8G8B8A8_UNORM:
        case EFormat::R8G8B8A8_SNORM:
        case EFormat::R8G8B8A8_SRGB:
        case EFormat::R16G16B16A16_USHORT:
        case EFormat::R16G16B16A16_SHORT:
        case EFormat::R32G32B32A32_UINT:
        case EFormat::R32G32B32A32_SINT:
        case EFormat::R32G32B32A32_FLOAT:
        case EFormat::R64G64B64A64_FLOAT:
            return false;
        case EFormat::D32_FLOAT:
        case EFormat::D32_FLOAT_S8_UINT:
        case EFormat::D24_UNORM_S8_UINT:
            return true;
        default: return false;
        }
    }
}