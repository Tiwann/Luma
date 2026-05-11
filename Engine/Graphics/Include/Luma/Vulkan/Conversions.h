#pragma once
#include "Luma/Rendering/Filter.h"
#include "Luma/Rendering/BlendFactor.h"
#include "Luma/Rendering/BlendOperation.h"
#include "Luma/Rendering/CompareOperation.h"
#include "Luma/Rendering/CullMode.h"
#include "Luma/Rendering/BindingType.h"
#include "Luma/Rendering/FrontFace.h"
#include "Luma/Rendering/PolygonMode.h"
#include "Luma/Rendering/PrimitiveTopology.h"
#include "Luma/Rendering/SamplerAddressMode.h"
#include "Luma/Rendering/PresentMode.h"
#include "Luma/Rendering/ShaderStage.h"
#include "Luma/Rendering/LoadOperation.h"
#include "Luma/Rendering/ResolveMode.h"
#include "Luma/Rendering/StoreOperation.h"
#include "Luma/Rendering/ResourceState.h"
#include "Luma/Rendering/ResourceBarrier.h"
#include "Luma/Rendering/TextureUsage.h"
#include "Luma/Rendering/TextureAspect.h"
#include "Luma/Rendering/TextureDimension.h"
#include "Luma/Rendering/VertexInputRate.h"
#include "Luma/Rendering/ComponentSwizzle.h"
#include "Luma/Rendering/ComponentMapping.h"
#include "Luma/Runtime/Format.h"
#include "Luma/Rendering/IndexFormat.h"
#include "Luma/Rendering/ShaderDataType.h"

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include "Luma/Rendering/RenderPassDesc.h"
#include "Luma/Rendering/TextureSubresourceRange.h"

namespace Luma::Vulkan
{
    template<typename VulkanType, typename NovaType>
    VulkanType convert(const NovaType& value);

    template<>
    inline VkFormat convert(const EFormat& value)
    {
        switch (value)
        {
        case EFormat::None:                  return VK_FORMAT_UNDEFINED;
        case EFormat::R8_UNORM:              return VK_FORMAT_R8_UNORM;
        case EFormat::R8_SNORM:              return VK_FORMAT_R8_SNORM;
        case EFormat::R16_USHORT:            return VK_FORMAT_R16_UINT;
        case EFormat::R16_SHORT:             return VK_FORMAT_R16_SINT;
        case EFormat::R32_FLOAT:             return VK_FORMAT_R32_SFLOAT;
        case EFormat::R32_UINT:              return VK_FORMAT_R32_UINT;
        case EFormat::R32_SINT:              return VK_FORMAT_R32_SINT;
        case EFormat::R8G8_UNORM:            return VK_FORMAT_R8G8_UNORM;
        case EFormat::R8G8_SNORM:            return VK_FORMAT_R8G8_SNORM;
        case EFormat::R16G16_USHORT:         return VK_FORMAT_R16G16_UINT;
        case EFormat::R16G16_SHORT:          return VK_FORMAT_R16G16_SINT;
        case EFormat::R32G32_UINT:           return VK_FORMAT_R32G32_UINT;
        case EFormat::R32G32_SINT:           return VK_FORMAT_R32G32_SINT;
        case EFormat::R32G32_FLOAT:          return VK_FORMAT_R32G32_SFLOAT;
        case EFormat::R8G8B8_UNORM:          return VK_FORMAT_R8G8B8_UNORM;
        case EFormat::R8G8B8_SNORM:          return VK_FORMAT_R8G8B8_SNORM;
        case EFormat::R16G16B16_USHORT:      return VK_FORMAT_R16G16B16_UINT;
        case EFormat::R16G16B16_SHORT:       return VK_FORMAT_R16G16B16_SINT;
        case EFormat::R32G32B32_UINT:        return VK_FORMAT_R32G32B32_UINT;
        case EFormat::R32G32B32_SINT:        return VK_FORMAT_R32G32B32_SINT;
        case EFormat::R32G32B32_FLOAT:       return VK_FORMAT_R32G32B32_SFLOAT;
        case EFormat::R8G8B8A8_UNORM:        return VK_FORMAT_R8G8B8A8_UNORM;
        case EFormat::R8G8B8A8_SNORM:        return VK_FORMAT_R8G8B8A8_SNORM;
        case EFormat::R8G8B8A8_SRGB:         return VK_FORMAT_R8G8B8A8_SRGB;
        case EFormat::R16G16B16A16_USHORT:   return VK_FORMAT_R16G16B16A16_UINT;
        case EFormat::R16G16B16A16_SHORT:    return VK_FORMAT_R16G16B16A16_SINT;
        case EFormat::R32G32B32A32_UINT:     return VK_FORMAT_R32G32B32A32_UINT;
        case EFormat::R32G32B32A32_SINT:     return VK_FORMAT_R32G32B32A32_SINT;
        case EFormat::R32G32B32A32_FLOAT:    return VK_FORMAT_R32G32B32A32_SFLOAT;
        case EFormat::R64G64B64A64_FLOAT:    return VK_FORMAT_R64G64B64A64_SFLOAT;
        case EFormat::D32_FLOAT: return VK_FORMAT_D32_SFLOAT;
        case EFormat::D32_FLOAT_S8_UINT: return VK_FORMAT_D32_SFLOAT_S8_UINT;
        case EFormat::D24_UNORM_S8_UINT: return VK_FORMAT_D24_UNORM_S8_UINT;

        // Compressed formats
        case EFormat::BC1_UNORM: return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
        case EFormat::BC1_SRGB: return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
        case EFormat::BC2_UNORM: return VK_FORMAT_BC2_UNORM_BLOCK;
        case EFormat::BC2_SRGB: return VK_FORMAT_BC2_SRGB_BLOCK;
        case EFormat::BC3_UNORM: return VK_FORMAT_BC3_UNORM_BLOCK;
        case EFormat::BC3_SRGB: return VK_FORMAT_BC3_SRGB_BLOCK;
        case EFormat::BC4_UNORM: return VK_FORMAT_BC4_UNORM_BLOCK;
        case EFormat::BC4_SNORM: return VK_FORMAT_BC4_SNORM_BLOCK;
        case EFormat::BC5_UNORM: return VK_FORMAT_BC5_UNORM_BLOCK;
        case EFormat::BC5_SNORM: return VK_FORMAT_BC5_SNORM_BLOCK;
        case EFormat::BC7_UNORM: return VK_FORMAT_BC7_UNORM_BLOCK;
        case EFormat::BC7_SRGB: return VK_FORMAT_BC7_SRGB_BLOCK;

        // ASTC formats

        case EFormat::ASTC_4X4_UNORM: return VK_FORMAT_ASTC_4x4_UNORM_BLOCK;
        case EFormat::ASTC_4X4_FLOAT: return VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK;
        case EFormat::ASTC_4X4_SRGB: return VK_FORMAT_ASTC_4x4_SRGB_BLOCK;

        case EFormat::ASTC_5X4_UNORM: return VK_FORMAT_ASTC_5x4_UNORM_BLOCK;
        case EFormat::ASTC_5X4_FLOAT: return VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK;
        case EFormat::ASTC_5X4_SRGB: return VK_FORMAT_ASTC_5x4_SRGB_BLOCK;

        case EFormat::ASTC_5X5_UNORM: return VK_FORMAT_ASTC_5x5_UNORM_BLOCK;
        case EFormat::ASTC_5X5_FLOAT: return VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK;
        case EFormat::ASTC_5X5_SRGB: return VK_FORMAT_ASTC_5x5_SRGB_BLOCK;

        case EFormat::ASTC_6X5_UNORM: return VK_FORMAT_ASTC_6x5_UNORM_BLOCK;
        case EFormat::ASTC_6X5_FLOAT: return VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK;
        case EFormat::ASTC_6X5_SRGB: return VK_FORMAT_ASTC_6x5_SRGB_BLOCK;

        case EFormat::ASTC_6X6_UNORM: return VK_FORMAT_ASTC_6x6_UNORM_BLOCK;
        case EFormat::ASTC_6X6_FLOAT: return VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK;
        case EFormat::ASTC_6X6_SRGB: return VK_FORMAT_ASTC_6x6_SRGB_BLOCK;

        case EFormat::ASTC_8X5_UNORM: return VK_FORMAT_ASTC_8x5_UNORM_BLOCK;
        case EFormat::ASTC_8X5_FLOAT: return VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK;
        case EFormat::ASTC_8X5_SRGB: return VK_FORMAT_ASTC_8x5_SRGB_BLOCK;

        case EFormat::ASTC_8X6_UNORM: return VK_FORMAT_ASTC_8x6_UNORM_BLOCK;
        case EFormat::ASTC_8X6_FLOAT: return VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK;
        case EFormat::ASTC_8X6_SRGB: return VK_FORMAT_ASTC_8x6_SRGB_BLOCK;

        case EFormat::ASTC_8X8_UNORM: return VK_FORMAT_ASTC_8x8_UNORM_BLOCK;
        case EFormat::ASTC_8X8_FLOAT: return VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK;
        case EFormat::ASTC_8X8_SRGB: return VK_FORMAT_ASTC_8x8_SRGB_BLOCK;

        case EFormat::ASTC_10X5_UNORM: return VK_FORMAT_ASTC_10x5_UNORM_BLOCK;
        case EFormat::ASTC_10X5_FLOAT: return VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK;
        case EFormat::ASTC_10X5_SRGB: return VK_FORMAT_ASTC_10x5_SRGB_BLOCK;

        case EFormat::ASTC_10X6_UNORM: return VK_FORMAT_ASTC_10x6_UNORM_BLOCK;
        case EFormat::ASTC_10X6_FLOAT: return VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK;
        case EFormat::ASTC_10X6_SRGB: return VK_FORMAT_ASTC_10x6_SRGB_BLOCK;

        case EFormat::ASTC_10X8_UNORM: return VK_FORMAT_ASTC_10x8_UNORM_BLOCK;
        case EFormat::ASTC_10X8_FLOAT: return VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK;
        case EFormat::ASTC_10X8_SRGB: return VK_FORMAT_ASTC_10x8_SRGB_BLOCK;

        case EFormat::ASTC_10X10_UNORM: return VK_FORMAT_ASTC_10x10_UNORM_BLOCK;
        case EFormat::ASTC_10X10_FLOAT: return VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK;
        case EFormat::ASTC_10X10_SRGB: return VK_FORMAT_ASTC_10x10_SRGB_BLOCK;

        case EFormat::ASTC_12X10_UNORM: return VK_FORMAT_ASTC_12x10_UNORM_BLOCK;
        case EFormat::ASTC_12X10_FLOAT: return VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK;
        case EFormat::ASTC_12X10_SRGB: return VK_FORMAT_ASTC_12x10_SRGB_BLOCK;

        case EFormat::ASTC_12X12_UNORM: return VK_FORMAT_ASTC_12x12_UNORM_BLOCK;
        case EFormat::ASTC_12X12_FLOAT: return VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK;
        case EFormat::ASTC_12X12_SRGB: return VK_FORMAT_ASTC_12x12_SRGB_BLOCK;
        default: return VK_FORMAT_UNDEFINED;
        }
    }

    template<>
    inline VkPresentModeKHR convert(const EPresentMode& value)
    {
        switch (value)
        {
        case EPresentMode::Unknown: throw;
        case EPresentMode::Immediate: return VK_PRESENT_MODE_IMMEDIATE_KHR;
        case EPresentMode::Mailbox: return VK_PRESENT_MODE_MAILBOX_KHR;
        case EPresentMode::Fifo: return VK_PRESENT_MODE_FIFO_KHR;
        default: throw;
        }
    }

    template<>
    inline VkFilter convert(const EFilter& filter)
    {
        switch (filter)
        {
        case EFilter::Nearest: return VK_FILTER_NEAREST;
        case EFilter::Linear: return VK_FILTER_LINEAR;
        default: return VK_FILTER_NEAREST;
        }
    }

    template<>
    inline VkPrimitiveTopology convert(const EPrimitiveTopology& value)
    {
        switch (value)
        {
        case EPrimitiveTopology::PointList: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
        case EPrimitiveTopology::LineList: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
        case EPrimitiveTopology::LineStrip: return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
        case EPrimitiveTopology::TriangleList: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        case EPrimitiveTopology::TriangleStrip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
        case EPrimitiveTopology::TriangleFan: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
        default: throw;
        }
    }

    template<>
    inline VkCullModeFlags convert(const ECullMode& value)
    {
        switch (value)
        {
        case ECullMode::FrontFace:           return VK_CULL_MODE_FRONT_BIT;
        case ECullMode::BackFace:            return VK_CULL_MODE_BACK_BIT;
        case ECullMode::FrontAndBackFaces:   return VK_CULL_MODE_FRONT_BIT | VK_CULL_MODE_BACK_BIT;
        case ECullMode::None:                return VK_CULL_MODE_NONE;
        default: return VK_CULL_MODE_NONE;
        }
    }

    template<>
    inline VkFrontFace convert(const EFrontFace& value)
    {
        switch (value)
        {
        case EFrontFace::Clockwise: return VK_FRONT_FACE_CLOCKWISE;
        case EFrontFace::CounterClockwise: return VK_FRONT_FACE_COUNTER_CLOCKWISE;
        default: throw;
        }
    }

    template<>
    inline VkPolygonMode convert(const EPolygonMode& value)
    {
        switch (value)
        {
        case EPolygonMode::Fill:; return VK_POLYGON_MODE_FILL;
        case EPolygonMode::Line: return VK_POLYGON_MODE_LINE;
        case EPolygonMode::Point: return VK_POLYGON_MODE_POINT;
        default: throw;
        }
    }

    template<>
    inline VkCompareOp convert(const ECompareOp& value)
    {
        switch (value)
        {
        case ECompareOp::Always:         return VK_COMPARE_OP_ALWAYS;
        case ECompareOp::Never:          return VK_COMPARE_OP_NEVER;
        case ECompareOp::Less:           return VK_COMPARE_OP_LESS;
        case ECompareOp::LessOrEqual:    return VK_COMPARE_OP_LESS_OR_EQUAL;
        case ECompareOp::Equal:          return VK_COMPARE_OP_EQUAL;
        case ECompareOp::NotEqual:       return VK_COMPARE_OP_NOT_EQUAL;
        case ECompareOp::Greater:        return VK_COMPARE_OP_GREATER;
        case ECompareOp::GreaterOrEqual: return VK_COMPARE_OP_GREATER_OR_EQUAL;
        }
        throw;
    }

    template<>
    inline VkSamplerAddressMode convert(const ESamplerAddressMode& value)
    {
        switch (value)
        {
        case ESamplerAddressMode::Repeat: return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        case ESamplerAddressMode::MirroredRepeat: return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        case ESamplerAddressMode::ClampToEdge: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        case ESamplerAddressMode::ClampToBorder: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        case ESamplerAddressMode::MirrorClampToEdge: return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
        }
        throw;
    }

    template<>
    inline VkBlendOp convert(const EBlendOp& value)
    {
        switch (value)
        {
        case EBlendOp::Add: return VK_BLEND_OP_ADD;
        case EBlendOp::Min: return VK_BLEND_OP_MIN;
        case EBlendOp::Max: return VK_BLEND_OP_MAX;
        case EBlendOp::Subtract: return VK_BLEND_OP_SUBTRACT;
        case EBlendOp::ReverseSubtract: return VK_BLEND_OP_REVERSE_SUBTRACT;
        }
        throw;
    }

    template<>
    inline VkBlendFactor convert(const EBlendFactor& value)
    {
        switch (value) {
        case EBlendFactor::Zero: return VK_BLEND_FACTOR_ZERO;
        case EBlendFactor::One: return VK_BLEND_FACTOR_ONE;
        case EBlendFactor::SourceColor: return VK_BLEND_FACTOR_SRC_COLOR;
        case EBlendFactor::OneMinusSourceColor: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
        case EBlendFactor::DestColor: return VK_BLEND_FACTOR_DST_COLOR;
        case EBlendFactor::OneMinusDestColor: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
        case EBlendFactor::SourceAlpha: return VK_BLEND_FACTOR_SRC_ALPHA;
        case EBlendFactor::OneMinusSourceAlpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        case EBlendFactor::DestAlpha: return VK_BLEND_FACTOR_DST_ALPHA;
        case EBlendFactor::OneMinusDestAlpha: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
        case EBlendFactor::ConstantColor: return VK_BLEND_FACTOR_CONSTANT_COLOR;
        case EBlendFactor::OneMinusConstantColor: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
        case EBlendFactor::ConstantAlpha: return VK_BLEND_FACTOR_CONSTANT_ALPHA;
        case EBlendFactor::OneMinusConstantAlpha: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
        case EBlendFactor::SourceAlphaSaturated: return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
        case EBlendFactor::Source1Color: return VK_BLEND_FACTOR_SRC1_COLOR;
        case EBlendFactor::OneMinusSource1Color: return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
        case EBlendFactor::Source1Alpha: return VK_BLEND_FACTOR_SRC1_ALPHA;
        case EBlendFactor::OneMinusSource1Alpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
        }
        throw;
    }

    template<>
    inline VkIndexType convert(const EFormat& value)
    {
        switch (value)
        {
        case EFormat::R32_UINT: return VK_INDEX_TYPE_UINT32;
        case EFormat::R16_USHORT : return VK_INDEX_TYPE_UINT16;
        case EFormat::R8_UNORM : return VK_INDEX_TYPE_UINT8;
        default: throw;
        }
    }

    template<>
    inline VkDescriptorType convert(const EBindingType& value)
    {
        switch (value)
        {
        case EBindingType::Sampler: return VK_DESCRIPTOR_TYPE_SAMPLER;
        case EBindingType::CombinedTextureSampler: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        case EBindingType::SampledTexture: return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        case EBindingType::StorageTexture: return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        case EBindingType::UniformBuffer: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        case EBindingType::StorageBuffer: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        case EBindingType::InputAttachment: return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
        case EBindingType::InlineUniformBlock: return VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK;
        case EBindingType::AccelerationStructure: return VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
        case EBindingType::StorageTexelBuffer: return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
        case EBindingType::UniformTexelBuffer: return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
        default: throw;
        }
    }

    template<>
    inline VkSamplerMipmapMode convert(const EFilter& value)
    {
        switch (value)
        {
        case EFilter::Nearest: return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        case EFilter::Linear: return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        default: throw;
        }
    }

    template<>
    inline VkAttachmentLoadOp convert(const ELoadOp& value)
    {
        switch (value)
        {
        case ELoadOp::Load: return VK_ATTACHMENT_LOAD_OP_LOAD;
        case ELoadOp::Clear: return VK_ATTACHMENT_LOAD_OP_CLEAR;
        case ELoadOp::DontCare: return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        default: throw;
        }
    }

    template<>
    inline VkAttachmentStoreOp convert(const EStoreOp& value)
    {
        switch (value)
        {
        case EStoreOp::Store: return VK_ATTACHMENT_STORE_OP_STORE;
        case EStoreOp::DontCare: return VK_ATTACHMENT_STORE_OP_DONT_CARE;
        default: throw;
        }
    }


    template<>
    inline VkShaderStageFlags convert(const FShaderStageFlags& value)
    {
        VkShaderStageFlags result = 0;
        const uint32_t val = value;
        if (val & (uint32_t)EShaderStageBits::Vertex)
            result |= VK_SHADER_STAGE_VERTEX_BIT;
        if (val & (uint32_t)EShaderStageBits::Fragment)
            result |= VK_SHADER_STAGE_FRAGMENT_BIT;
        if (val & (uint32_t)EShaderStageBits::Geometry)
            result |= VK_SHADER_STAGE_GEOMETRY_BIT;
        if (val & (uint32_t)EShaderStageBits::TessellationControl)
            result |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        if (val & (uint32_t)EShaderStageBits::TessellationEvaluation)
            result |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        if (val & (uint32_t)EShaderStageBits::Mesh)
            result |= VK_SHADER_STAGE_MESH_BIT_EXT;
        if (val & (uint32_t)EShaderStageBits::Compute)
            result |= VK_SHADER_STAGE_COMPUTE_BIT;
        if (val & (uint32_t)EShaderStageBits::RayGeneration)
            result |= VK_SHADER_STAGE_RAYGEN_BIT_KHR;
        return result;
    }

    template<>
    inline VkShaderStageFlagBits convert(const EShaderStageBits& value)
    {
        switch (value)
        {
        case EShaderStageBits::None: return (VkShaderStageFlagBits)0;
        case EShaderStageBits::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
        case EShaderStageBits::Geometry: return VK_SHADER_STAGE_GEOMETRY_BIT;
        case EShaderStageBits::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
        case EShaderStageBits::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
        case EShaderStageBits::RayGeneration: return VK_SHADER_STAGE_RAYGEN_BIT_KHR;
        case EShaderStageBits::TessellationControl: return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        case EShaderStageBits::Mesh: return VK_SHADER_STAGE_MESH_BIT_EXT;
        case EShaderStageBits::TessellationEvaluation: return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        default: return (VkShaderStageFlagBits)0;
        }
    }

    template<>
    inline VkResolveModeFlagBits convert(const EResolveMode& value)
    {
        switch (value)
        {
        case EResolveMode::None: return VK_RESOLVE_MODE_NONE;
        case EResolveMode::Zero: return VK_RESOLVE_MODE_SAMPLE_ZERO_BIT;
        case EResolveMode::Average: return VK_RESOLVE_MODE_AVERAGE_BIT;
        case EResolveMode::Min: return VK_RESOLVE_MODE_MIN_BIT;
        case EResolveMode::Max: return VK_RESOLVE_MODE_MAX_BIT;
        default: return VK_RESOLVE_MODE_NONE;
        }
    }

    template<>
    inline VkAccessFlags convert(const FResourceAccessFlags& value)
    {
        VkAccessFlags result = VK_ACCESS_NONE;
        if (value == EResourceAccessBits::None)
            return VK_ACCESS_NONE;

        if (value & EResourceAccessBits::ShaderRead)
            result |= VK_ACCESS_SHADER_READ_BIT;
        if (value & EResourceAccessBits::ShaderWrite)
            result |= VK_ACCESS_SHADER_WRITE_BIT;
        if (value & EResourceAccessBits::ColorAttachmentRead)
            result |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
        if (value & EResourceAccessBits::ColorAttachmentWrite)
            result |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        if (value & EResourceAccessBits::DepthStencilAttachmentRead)
            result |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
        if (value & EResourceAccessBits::DepthStencilAttachmentWrite)
            result |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        if (value & EResourceAccessBits::TransferRead)
            result |= VK_ACCESS_TRANSFER_READ_BIT;
        if (value & EResourceAccessBits::TransferWrite)
            result |= VK_ACCESS_TRANSFER_WRITE_BIT;
        if (value & EResourceAccessBits::HostRead)
            result |= VK_ACCESS_HOST_READ_BIT;
        if (value & EResourceAccessBits::HostWrite)
            result |= VK_ACCESS_HOST_WRITE_BIT;

        return result;
    }

    template<>
    inline VkImageLayout convert(const EResourceState& value)
    {
        switch (value)
        {
        case EResourceState::Undefined: return VK_IMAGE_LAYOUT_UNDEFINED;
        case EResourceState::General: return VK_IMAGE_LAYOUT_GENERAL;
        case EResourceState::ShaderRead: return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        case EResourceState::ShaderWrite: return VK_IMAGE_LAYOUT_GENERAL;
        case EResourceState::ColorAttachment: return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        case EResourceState::DepthStencilAttachment: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        case EResourceState::CopySource: return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        case EResourceState::CopyDest: return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        case EResourceState::Present: return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        default: return VK_IMAGE_LAYOUT_UNDEFINED;
        }
    }

    template<>
    inline VkImageUsageFlags convert(const FTextureUsageFlags& value)
    {
        VkImageUsageFlags flags = 0;
        if (value == ETextureUsageBits::None)
            return 0;
        if (value & ETextureUsageBits::Sampled)
            flags |= VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        if (value & ETextureUsageBits::Storage)
            flags |= VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        if (value & ETextureUsageBits::ColorAttachment)
            flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        if (value & ETextureUsageBits::DepthStencilAttachment)
            flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        return flags;
    }

    template<>
    inline VkImageAspectFlags convert(const FTextureAspectFlags& value)
    {
        VkImageAspectFlags flags = 0;
        if (value == ETextureAspectBits::None)
            return VK_IMAGE_ASPECT_NONE;
        if (value & ETextureAspectBits::Color)
            flags |= VK_IMAGE_ASPECT_COLOR_BIT;
        if (value & ETextureAspectBits::Depth)
            flags |= VK_IMAGE_ASPECT_DEPTH_BIT;
        if (value & ETextureAspectBits::Stencil)
            flags |= VK_IMAGE_ASPECT_STENCIL_BIT;
        return flags;
    }

    inline VkImageAspectFlags convert(const EFormat& value)
    {
        VkImageAspectFlags flags = 0;
        switch (value)
        {
        case EFormat::None: return 0;
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
            return VK_IMAGE_ASPECT_COLOR_BIT;
        case EFormat::D32_FLOAT:
            return VK_IMAGE_ASPECT_DEPTH_BIT;
        case EFormat::D32_FLOAT_S8_UINT:
            return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
        case EFormat::D24_UNORM_S8_UINT:
            return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
        default: return VK_IMAGE_ASPECT_COLOR_BIT;
        }
    }

    /*template<>
    inline VkImageLayout convert(const RenderPassAttachmentType& value)
    {
        switch (value)
        {
        case RenderPassAttachmentType::Color: return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        case RenderPassAttachmentType::Depth: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        default: return VK_IMAGE_LAYOUT_UNDEFINED;
        }
    }*/

    template<>
    inline VkImageType convert(const ETextureDimension& value)
    {
        switch (value)
        {
        case ETextureDimension::None: return VK_IMAGE_TYPE_MAX_ENUM;
        case ETextureDimension::Dim1D: return VK_IMAGE_TYPE_1D;
        case ETextureDimension::Dim2D: return VK_IMAGE_TYPE_2D;
        case ETextureDimension::Dim3D: return VK_IMAGE_TYPE_3D;
        default: return VK_IMAGE_TYPE_MAX_ENUM;
        }
    }

    template<>
    inline VkVertexInputRate convert(const EVertexInputRate& value)
    {
        switch (value)
        {
        case EVertexInputRate::Vertex: return VK_VERTEX_INPUT_RATE_VERTEX;
        case EVertexInputRate::Instance: return VK_VERTEX_INPUT_RATE_INSTANCE;
        default: return VK_VERTEX_INPUT_RATE_VERTEX;
        }
    }

    template<>
    inline VkComponentSwizzle convert(const EComponentSwizzle& value)
    {
        switch (value)
        {
        case EComponentSwizzle::Identity: return VK_COMPONENT_SWIZZLE_IDENTITY;
        case EComponentSwizzle::Zero: return VK_COMPONENT_SWIZZLE_ZERO;
        case EComponentSwizzle::One: return VK_COMPONENT_SWIZZLE_ONE;
        case EComponentSwizzle::Red: return VK_COMPONENT_SWIZZLE_R;
        case EComponentSwizzle::Green: return VK_COMPONENT_SWIZZLE_G;
        case EComponentSwizzle::Blue: return VK_COMPONENT_SWIZZLE_B;
        case EComponentSwizzle::Alpha: return VK_COMPONENT_SWIZZLE_A;
        default: return VK_COMPONENT_SWIZZLE_IDENTITY;
        }
    }

    template<>
    inline VkComponentMapping convert(const FComponentMapping& value)
    {
        VkComponentMapping mapping;
        mapping.r = convert<VkComponentSwizzle>(value.r);
        mapping.g = convert<VkComponentSwizzle>(value.g);
        mapping.b = convert<VkComponentSwizzle>(value.b);
        mapping.a = convert<VkComponentSwizzle>(value.a);
        return mapping;
    }

    template <>
    inline VkIndexType convert(const EIndexFormat& value)
    {
        switch (value)
        {
        case EIndexFormat::Uint32: return VK_INDEX_TYPE_UINT32;
        case EIndexFormat::Uint16: return VK_INDEX_TYPE_UINT16;
        case EIndexFormat::Uint8: return VK_INDEX_TYPE_UINT8;
        default: return VK_INDEX_TYPE_UINT32;
        }
    }

    template<>
    inline VkFormat convert(const EShaderDataType& value)
    {
        switch (value)
        {
        case EShaderDataType::Float: return VK_FORMAT_R32_SFLOAT;
        case EShaderDataType::Float2: return VK_FORMAT_R32G32_SFLOAT;
        case EShaderDataType::Float3: return VK_FORMAT_R32G32B32_SFLOAT;
        case EShaderDataType::Float4: return VK_FORMAT_R32G32B32A32_SFLOAT;
        case EShaderDataType::Int: return VK_FORMAT_R32_SINT;
        case EShaderDataType::Int2: return VK_FORMAT_R32G32_SINT;
        case EShaderDataType::Int3: return VK_FORMAT_R32G32B32_SINT;
        case EShaderDataType::Int4: return VK_FORMAT_R32G32B32A32_SINT;
        case EShaderDataType::UInt: return VK_FORMAT_R32_UINT;
        case EShaderDataType::UInt2: return VK_FORMAT_R32G32_UINT;
        case EShaderDataType::UInt3: return VK_FORMAT_R32G32B32_UINT;
        case EShaderDataType::UInt4: return VK_FORMAT_R32G32B32A32_UINT;
            default: return VK_FORMAT_UNDEFINED;
        }
    }

    template<>
    inline VkImageLayout convert(const ERenderPassAttachmentType& value)
    {
        switch (value)
        {
        case ERenderPassAttachmentType::Color: return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        case ERenderPassAttachmentType::DepthStencil: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        default: return VK_IMAGE_LAYOUT_UNDEFINED;
        }
    }

    template<>
    inline VkColorComponentFlags convert(const FColorChannelFlags& value)
    {
        VkColorComponentFlags result = 0;
        if (value & EColorChannelBits::Red) result |= VK_COLOR_COMPONENT_R_BIT;
        if (value & EColorChannelBits::Green) result |= VK_COLOR_COMPONENT_G_BIT;
        if (value & EColorChannelBits::Blue) result |= VK_COLOR_COMPONENT_B_BIT;
        if (value & EColorChannelBits::Alpha) result |= VK_COLOR_COMPONENT_A_BIT;
        return result;
    }

    template<>
    inline VkImageSubresourceRange convert(const FTextureSubresourceRange& value)
    {
        VkImageSubresourceRange range;
        range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        range.baseMipLevel = value.baseMipLevel;
        range.levelCount = value.mipCount;
        range.baseArrayLayer = value.baseArrayLayer;
        range.layerCount = value.layerCount;
        return range;
    }
}
