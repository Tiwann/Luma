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
#include "Luma/Rendering/RenderPassDesc.h"
#include "Luma/Rendering/TextureSubresourceRange.h"
#include "Luma/Rendering/TextureBinding.h"
#include "Luma/Rendering/BufferBinding.h"
#include "Luma/Rendering/ShaderBinding.h"
#include "Luma/Rendering/ShaderPushConstantVariable.h"

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>



namespace Luma::Vulkan
{
    template<typename VulkanType, typename NovaType>
    VulkanType convert(const NovaType& value);

    template<>
    inline VkFormat convert(const Format& value)
    {
        switch (value)
        {
        case Format::None:                  return VK_FORMAT_UNDEFINED;
        case Format::R8_UNORM:              return VK_FORMAT_R8_UNORM;
        case Format::R8_SNORM:              return VK_FORMAT_R8_SNORM;
        case Format::R16_USHORT:            return VK_FORMAT_R16_UINT;
        case Format::R16_SHORT:             return VK_FORMAT_R16_SINT;
        case Format::R32_FLOAT:             return VK_FORMAT_R32_SFLOAT;
        case Format::R32_UINT:              return VK_FORMAT_R32_UINT;
        case Format::R32_SINT:              return VK_FORMAT_R32_SINT;
        case Format::R8G8_UNORM:            return VK_FORMAT_R8G8_UNORM;
        case Format::R8G8_SNORM:            return VK_FORMAT_R8G8_SNORM;
        case Format::R16G16_USHORT:         return VK_FORMAT_R16G16_UINT;
        case Format::R16G16_SHORT:          return VK_FORMAT_R16G16_SINT;
        case Format::R32G32_UINT:           return VK_FORMAT_R32G32_UINT;
        case Format::R32G32_SINT:           return VK_FORMAT_R32G32_SINT;
        case Format::R32G32_FLOAT:          return VK_FORMAT_R32G32_SFLOAT;
        case Format::R8G8B8_UNORM:          return VK_FORMAT_R8G8B8_UNORM;
        case Format::R8G8B8_SNORM:          return VK_FORMAT_R8G8B8_SNORM;
        case Format::R16G16B16_USHORT:      return VK_FORMAT_R16G16B16_UINT;
        case Format::R16G16B16_SHORT:       return VK_FORMAT_R16G16B16_SINT;
        case Format::R32G32B32_UINT:        return VK_FORMAT_R32G32B32_UINT;
        case Format::R32G32B32_SINT:        return VK_FORMAT_R32G32B32_SINT;
        case Format::R32G32B32_FLOAT:       return VK_FORMAT_R32G32B32_SFLOAT;
        case Format::R8G8B8A8_UNORM:        return VK_FORMAT_R8G8B8A8_UNORM;
        case Format::R8G8B8A8_SNORM:        return VK_FORMAT_R8G8B8A8_SNORM;
        case Format::R8G8B8A8_SRGB:         return VK_FORMAT_R8G8B8A8_SRGB;
        case Format::R16G16B16A16_USHORT:   return VK_FORMAT_R16G16B16A16_UINT;
        case Format::R16G16B16A16_SHORT:    return VK_FORMAT_R16G16B16A16_SINT;
        case Format::R32G32B32A32_UINT:     return VK_FORMAT_R32G32B32A32_UINT;
        case Format::R32G32B32A32_SINT:     return VK_FORMAT_R32G32B32A32_SINT;
        case Format::R32G32B32A32_FLOAT:    return VK_FORMAT_R32G32B32A32_SFLOAT;
        case Format::R64G64B64A64_FLOAT:    return VK_FORMAT_R64G64B64A64_SFLOAT;
        case Format::D32_FLOAT: return VK_FORMAT_D32_SFLOAT;
        case Format::D32_FLOAT_S8_UINT: return VK_FORMAT_D32_SFLOAT_S8_UINT;
        case Format::D24_UNORM_S8_UINT: return VK_FORMAT_D24_UNORM_S8_UINT;

        // Compressed formats
        case Format::BC1_UNORM: return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
        case Format::BC1_SRGB: return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
        case Format::BC2_UNORM: return VK_FORMAT_BC2_UNORM_BLOCK;
        case Format::BC2_SRGB: return VK_FORMAT_BC2_SRGB_BLOCK;
        case Format::BC3_UNORM: return VK_FORMAT_BC3_UNORM_BLOCK;
        case Format::BC3_SRGB: return VK_FORMAT_BC3_SRGB_BLOCK;
        case Format::BC4_UNORM: return VK_FORMAT_BC4_UNORM_BLOCK;
        case Format::BC4_SNORM: return VK_FORMAT_BC4_SNORM_BLOCK;
        case Format::BC5_UNORM: return VK_FORMAT_BC5_UNORM_BLOCK;
        case Format::BC5_SNORM: return VK_FORMAT_BC5_SNORM_BLOCK;
        case Format::BC7_UNORM: return VK_FORMAT_BC7_UNORM_BLOCK;
        case Format::BC7_SRGB: return VK_FORMAT_BC7_SRGB_BLOCK;
        default: return VK_FORMAT_UNDEFINED;
        }
    }

    template<>
    inline VkPresentModeKHR convert(const PresentMode& value)
    {
        switch (value)
        {
        case PresentMode::Unknown: throw;
        case PresentMode::Immediate: return VK_PRESENT_MODE_IMMEDIATE_KHR;
        case PresentMode::Fifo: return VK_PRESENT_MODE_FIFO_KHR;
        default: throw;
        }
    }

    template<>
    inline VkFilter convert(const Filter& filter)
    {
        switch (filter)
        {
        case Filter::Nearest: return VK_FILTER_NEAREST;
        case Filter::Linear: return VK_FILTER_LINEAR;
        default: return VK_FILTER_NEAREST;
        }
    }

    template<>
    inline VkPrimitiveTopology convert(const PrimitiveTopology& value)
    {
        switch (value)
        {
        case PrimitiveTopology::PointList: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
        case PrimitiveTopology::LineList: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
        case PrimitiveTopology::LineStrip: return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
        case PrimitiveTopology::TriangleList: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        case PrimitiveTopology::TriangleStrip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
        case PrimitiveTopology::TriangleFan: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
        default: throw;
        }
    }

    template<>
    inline VkCullModeFlags convert(const CullMode& value)
    {
        switch (value)
        {
        case CullMode::FrontFace:           return VK_CULL_MODE_FRONT_BIT;
        case CullMode::BackFace:            return VK_CULL_MODE_BACK_BIT;
        case CullMode::FrontAndBackFaces:   return VK_CULL_MODE_FRONT_BIT | VK_CULL_MODE_BACK_BIT;
        case CullMode::None:                return VK_CULL_MODE_NONE;
        default: return VK_CULL_MODE_NONE;
        }
    }

    template<>
    inline VkFrontFace convert(const FrontFace& value)
    {
        switch (value)
        {
        case FrontFace::Clockwise: return VK_FRONT_FACE_CLOCKWISE;
        case FrontFace::CounterClockwise: return VK_FRONT_FACE_COUNTER_CLOCKWISE;
        default: throw;
        }
    }

    template<>
    inline VkPolygonMode convert(const PolygonMode& value)
    {
        switch (value)
        {
        case PolygonMode::Fill:; return VK_POLYGON_MODE_FILL;
        case PolygonMode::Line: return VK_POLYGON_MODE_LINE;
        case PolygonMode::Point: return VK_POLYGON_MODE_POINT;
        default: throw;
        }
    }

    template<>
    inline VkCompareOp convert(const CompareOp& value)
    {
        switch (value)
        {
        case CompareOp::Always:         return VK_COMPARE_OP_ALWAYS;
        case CompareOp::Never:          return VK_COMPARE_OP_NEVER;
        case CompareOp::Less:           return VK_COMPARE_OP_LESS;
        case CompareOp::LessOrEqual:    return VK_COMPARE_OP_LESS_OR_EQUAL;
        case CompareOp::Equal:          return VK_COMPARE_OP_EQUAL;
        case CompareOp::NotEqual:       return VK_COMPARE_OP_NOT_EQUAL;
        case CompareOp::Greater:        return VK_COMPARE_OP_GREATER;
        case CompareOp::GreaterOrEqual: return VK_COMPARE_OP_GREATER_OR_EQUAL;
        }
        throw;
    }

    template<>
    inline VkSamplerAddressMode convert(const SamplerAddressMode& value)
    {
        switch (value)
        {
        case SamplerAddressMode::Repeat: return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        case SamplerAddressMode::MirroredRepeat: return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        case SamplerAddressMode::ClampToEdge: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        case SamplerAddressMode::ClampToBorder: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        case SamplerAddressMode::MirrorClampToEdge: return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
        }
        throw;
    }

    template<>
    inline VkBlendOp convert(const BlendOp& value)
    {
        switch (value)
        {
        case BlendOp::Add: return VK_BLEND_OP_ADD;
        case BlendOp::Min: return VK_BLEND_OP_MIN;
        case BlendOp::Max: return VK_BLEND_OP_MAX;
        case BlendOp::Subtract: return VK_BLEND_OP_SUBTRACT;
        case BlendOp::ReverseSubtract: return VK_BLEND_OP_REVERSE_SUBTRACT;
        }
        throw;
    }

    template<>
    inline VkBlendFactor convert(const BlendFactor& value)
    {
        switch (value) {
        case BlendFactor::Zero: return VK_BLEND_FACTOR_ZERO;
        case BlendFactor::One: return VK_BLEND_FACTOR_ONE;
        case BlendFactor::SourceColor: return VK_BLEND_FACTOR_SRC_COLOR;
        case BlendFactor::OneMinusSourceColor: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
        case BlendFactor::DestColor: return VK_BLEND_FACTOR_DST_COLOR;
        case BlendFactor::OneMinusDestColor: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
        case BlendFactor::SourceAlpha: return VK_BLEND_FACTOR_SRC_ALPHA;
        case BlendFactor::OneMinusSourceAlpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        case BlendFactor::DestAlpha: return VK_BLEND_FACTOR_DST_ALPHA;
        case BlendFactor::OneMinusDestAlpha: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
        case BlendFactor::ConstantColor: return VK_BLEND_FACTOR_CONSTANT_COLOR;
        case BlendFactor::OneMinusConstantColor: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
        case BlendFactor::ConstantAlpha: return VK_BLEND_FACTOR_CONSTANT_ALPHA;
        case BlendFactor::OneMinusConstantAlpha: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
        case BlendFactor::SourceAlphaSaturated: return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
        case BlendFactor::Source1Color: return VK_BLEND_FACTOR_SRC1_COLOR;
        case BlendFactor::OneMinusSource1Color: return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
        case BlendFactor::Source1Alpha: return VK_BLEND_FACTOR_SRC1_ALPHA;
        case BlendFactor::OneMinusSource1Alpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
        }
        throw;
    }

    template<>
    inline VkIndexType convert(const Format& value)
    {
        switch (value)
        {
        case Format::R32_UINT: return VK_INDEX_TYPE_UINT32;
        case Format::R16_USHORT : return VK_INDEX_TYPE_UINT16;
        case Format::R8_UNORM : return VK_INDEX_TYPE_UINT8;
        default: throw;
        }
    }

    template<>
    inline VkDescriptorType convert(const BindingType& value)
    {
        switch (value)
        {
        case BindingType::Sampler: return VK_DESCRIPTOR_TYPE_SAMPLER;
        case BindingType::CombinedTextureSampler: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        case BindingType::SampledTexture: return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        case BindingType::StorageTexture: return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        case BindingType::UniformBuffer: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        case BindingType::StorageBuffer: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        case BindingType::InputTarget: return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
        case BindingType::InlineUniformBlock: return VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK;
        case BindingType::AccelerationStructure: return VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
        case BindingType::StorageTexelBuffer: return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
        case BindingType::UniformTexelBuffer: return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
        default: throw;
        }
    }

    template<>
    inline VkSamplerMipmapMode convert(const Filter& value)
    {
        switch (value)
        {
        case Filter::Nearest: return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        case Filter::Linear: return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        default: throw;
        }
    }

    template<>
    inline VkAttachmentLoadOp convert(const LoadOp& value)
    {
        switch (value)
        {
        case LoadOp::Load: return VK_ATTACHMENT_LOAD_OP_LOAD;
        case LoadOp::Clear: return VK_ATTACHMENT_LOAD_OP_CLEAR;
        case LoadOp::DontCare: return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        default: throw;
        }
    }

    template<>
    inline VkAttachmentStoreOp convert(const StoreOp& value)
    {
        switch (value)
        {
        case StoreOp::Store: return VK_ATTACHMENT_STORE_OP_STORE;
        case StoreOp::DontCare: return VK_ATTACHMENT_STORE_OP_DONT_CARE;
        default: throw;
        }
    }


    template<>
    inline VkShaderStageFlags convert(const ShaderStageFlags& value)
    {
        VkShaderStageFlags result = 0;
        const uint32_t val = value;
        if (val & (uint32_t)ShaderStage::Vertex)
            result |= VK_SHADER_STAGE_VERTEX_BIT;
        if (val & (uint32_t)ShaderStage::Fragment)
            result |= VK_SHADER_STAGE_FRAGMENT_BIT;
        if (val & (uint32_t)ShaderStage::Geometry)
            result |= VK_SHADER_STAGE_GEOMETRY_BIT;
        if (val & (uint32_t)ShaderStage::TessellationControl)
            result |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        if (val & (uint32_t)ShaderStage::TessellationEvaluation)
            result |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        if (val & (uint32_t)ShaderStage::Mesh)
            result |= VK_SHADER_STAGE_MESH_BIT_EXT;
        if (val & (uint32_t)ShaderStage::Compute)
            result |= VK_SHADER_STAGE_COMPUTE_BIT;
        return result;
    }

    template<>
    inline VkShaderStageFlagBits convert(const ShaderStage& value)
    {
        switch (value)
        {
        case ShaderStage::None: return (VkShaderStageFlagBits)0;
        case ShaderStage::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
        case ShaderStage::Geometry: return VK_SHADER_STAGE_GEOMETRY_BIT;
        case ShaderStage::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
        case ShaderStage::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
        case ShaderStage::TessellationControl: return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        case ShaderStage::Mesh: return VK_SHADER_STAGE_MESH_BIT_EXT;
        case ShaderStage::TessellationEvaluation: return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        default: return (VkShaderStageFlagBits)0;
        }
    }

    template<>
    inline VkResolveModeFlagBits convert(const ResolveMode& value)
    {
        switch (value)
        {
        case ResolveMode::None: return VK_RESOLVE_MODE_NONE;
        case ResolveMode::Zero: return VK_RESOLVE_MODE_SAMPLE_ZERO_BIT;
        case ResolveMode::Average: return VK_RESOLVE_MODE_AVERAGE_BIT;
        case ResolveMode::Min: return VK_RESOLVE_MODE_MIN_BIT;
        case ResolveMode::Max: return VK_RESOLVE_MODE_MAX_BIT;
        default: return VK_RESOLVE_MODE_NONE;
        }
    }

    template<>
    inline VkAccessFlags2 convert(const ResourceAccessFlags& value)
    {
        VkAccessFlags2 result = VK_ACCESS_2_NONE;
        if (value == ResourceAccess::None)
            return VK_ACCESS_2_NONE;

        if (value & ResourceAccess::ShaderRead)
            result |= VK_ACCESS_2_SHADER_READ_BIT;
        if (value & ResourceAccess::ShaderWrite)
            result |= VK_ACCESS_2_SHADER_WRITE_BIT;
        if (value & ResourceAccess::ColorTargetRead)
            result |= VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT;
        if (value & ResourceAccess::ColorTargetWrite)
            result |= VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
        if (value & ResourceAccess::DepthStencilTargetRead)
            result |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
        if (value & ResourceAccess::DepthStencilTargetWrite)
            result |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        if (value & ResourceAccess::CopyRead)
            result |= VK_ACCESS_2_TRANSFER_READ_BIT;
        if (value & ResourceAccess::CopyWrite)
            result |= VK_ACCESS_2_TRANSFER_WRITE_BIT;
        if (value & ResourceAccess::HostRead)
            result |= VK_ACCESS_2_HOST_READ_BIT;
        if (value & ResourceAccess::HostWrite)
            result |= VK_ACCESS_2_HOST_WRITE_BIT;

        return result;
    }

    template<>
    inline VkImageLayout convert(const ResourceState& value)
    {
        switch (value)
        {
        case ResourceState::Undefined: return VK_IMAGE_LAYOUT_UNDEFINED;
        case ResourceState::General: return VK_IMAGE_LAYOUT_GENERAL;
        case ResourceState::ShaderRead: return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        case ResourceState::ShaderWrite: return VK_IMAGE_LAYOUT_GENERAL;
        case ResourceState::ColorTarget: return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        case ResourceState::DepthStencilTarget: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        case ResourceState::CopySource: return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        case ResourceState::CopyDest: return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        case ResourceState::Present: return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        default: return VK_IMAGE_LAYOUT_UNDEFINED;
        }
    }

    template<>
    inline VkImageUsageFlags convert(const TextureUsageFlags& value)
    {
        VkImageUsageFlags flags = 0;
        if (value == TextureUsage::None)
            return 0;
        if (value & TextureUsage::Sampled)
            flags |= VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        if (value & TextureUsage::Storage)
            flags |= VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        if (value & (TextureUsage::Color | TextureUsage::RenderTarget))
            flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        if (value & (TextureUsage::DepthStencil | TextureUsage::RenderTarget))
            flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        return flags;
    }

    template<>
    inline VkImageAspectFlags convert(const TextureAspectFlags& value)
    {
        VkImageAspectFlags flags = 0;
        if (value == TextureAspect::None)
            return VK_IMAGE_ASPECT_NONE;
        if (value & TextureAspect::Color)
            flags |= VK_IMAGE_ASPECT_COLOR_BIT;
        if (value & TextureAspect::Depth)
            flags |= VK_IMAGE_ASPECT_DEPTH_BIT;
        if (value & TextureAspect::Stencil)
            flags |= VK_IMAGE_ASPECT_STENCIL_BIT;
        return flags;
    }

    inline VkImageAspectFlags convert(const Format& value)
    {
        VkImageAspectFlags flags = 0;
        switch (value)
        {
        case Format::None: return 0;
        case Format::R8_UNORM:
        case Format::R8_SNORM:
        case Format::R16_USHORT:
        case Format::R16_SHORT:
        case Format::R32_FLOAT:
        case Format::R32_UINT:
        case Format::R32_SINT:
        case Format::R8G8_UNORM:
        case Format::R8G8_SNORM:
        case Format::R16G16_USHORT:
        case Format::R16G16_SHORT:
        case Format::R32G32_UINT:
        case Format::R32G32_SINT:
        case Format::R32G32_FLOAT:
        case Format::R8G8B8_UNORM:
        case Format::R8G8B8_SNORM:
        case Format::R16G16B16_USHORT:
        case Format::R16G16B16_SHORT:
        case Format::R32G32B32_UINT:
        case Format::R32G32B32_SINT:
        case Format::R32G32B32_FLOAT:
        case Format::R8G8B8A8_UNORM:
        case Format::R8G8B8A8_SNORM:
        case Format::R8G8B8A8_SRGB:
        case Format::R16G16B16A16_USHORT:
        case Format::R16G16B16A16_SHORT:
        case Format::R32G32B32A32_UINT:
        case Format::R32G32B32A32_SINT:
        case Format::R32G32B32A32_FLOAT:
        case Format::R64G64B64A64_FLOAT:
            return VK_IMAGE_ASPECT_COLOR_BIT;
        case Format::D32_FLOAT:
            return VK_IMAGE_ASPECT_DEPTH_BIT;
        case Format::D32_FLOAT_S8_UINT:
            return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
        case Format::D24_UNORM_S8_UINT:
            return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
        default: return VK_IMAGE_ASPECT_COLOR_BIT;
        }
    }

    template<>
    inline VkImageType convert(const TextureDimension& value)
    {
        switch (value)
        {
        case TextureDimension::None: return VK_IMAGE_TYPE_MAX_ENUM;
        case TextureDimension::Dim1D: return VK_IMAGE_TYPE_1D;
        case TextureDimension::Dim2D: return VK_IMAGE_TYPE_2D;
        case TextureDimension::Dim3D: return VK_IMAGE_TYPE_3D;
        default: return VK_IMAGE_TYPE_MAX_ENUM;
        }
    }

    template<>
    inline VkVertexInputRate convert(const VertexInputRate& value)
    {
        switch (value)
        {
        case VertexInputRate::Vertex: return VK_VERTEX_INPUT_RATE_VERTEX;
        case VertexInputRate::Instance: return VK_VERTEX_INPUT_RATE_INSTANCE;
        default: return VK_VERTEX_INPUT_RATE_VERTEX;
        }
    }

    template<>
    inline VkComponentSwizzle convert(const ComponentSwizzle& value)
    {
        switch (value)
        {
        case ComponentSwizzle::Identity: return VK_COMPONENT_SWIZZLE_IDENTITY;
        case ComponentSwizzle::Zero: return VK_COMPONENT_SWIZZLE_ZERO;
        case ComponentSwizzle::One: return VK_COMPONENT_SWIZZLE_ONE;
        case ComponentSwizzle::Red: return VK_COMPONENT_SWIZZLE_R;
        case ComponentSwizzle::Green: return VK_COMPONENT_SWIZZLE_G;
        case ComponentSwizzle::Blue: return VK_COMPONENT_SWIZZLE_B;
        case ComponentSwizzle::Alpha: return VK_COMPONENT_SWIZZLE_A;
        default: return VK_COMPONENT_SWIZZLE_IDENTITY;
        }
    }

    template<>
    inline VkComponentMapping convert(const ComponentMapping& value)
    {
        VkComponentMapping mapping;
        mapping.r = convert<VkComponentSwizzle>(value.r);
        mapping.g = convert<VkComponentSwizzle>(value.g);
        mapping.b = convert<VkComponentSwizzle>(value.b);
        mapping.a = convert<VkComponentSwizzle>(value.a);
        return mapping;
    }

    template <>
    inline VkIndexType convert(const IndexFormat& value)
    {
        switch (value)
        {
        case IndexFormat::UInt32: return VK_INDEX_TYPE_UINT32;
        case IndexFormat::UInt16: return VK_INDEX_TYPE_UINT16;
        case IndexFormat::UInt8: return VK_INDEX_TYPE_UINT8;
        default: return VK_INDEX_TYPE_UINT32;
        }
    }

    template<>
    inline VkFormat convert(const ShaderDataType& value)
    {
        switch (value)
        {
        case ShaderDataType::Float: return VK_FORMAT_R32_SFLOAT;
        case ShaderDataType::Float2: return VK_FORMAT_R32G32_SFLOAT;
        case ShaderDataType::Float3: return VK_FORMAT_R32G32B32_SFLOAT;
        case ShaderDataType::Float4: return VK_FORMAT_R32G32B32A32_SFLOAT;
        case ShaderDataType::Int: return VK_FORMAT_R32_SINT;
        case ShaderDataType::Int2: return VK_FORMAT_R32G32_SINT;
        case ShaderDataType::Int3: return VK_FORMAT_R32G32B32_SINT;
        case ShaderDataType::Int4: return VK_FORMAT_R32G32B32A32_SINT;
        case ShaderDataType::UInt: return VK_FORMAT_R32_UINT;
        case ShaderDataType::UInt2: return VK_FORMAT_R32G32_UINT;
        case ShaderDataType::UInt3: return VK_FORMAT_R32G32B32_UINT;
        case ShaderDataType::UInt4: return VK_FORMAT_R32G32B32A32_UINT;
            default: return VK_FORMAT_UNDEFINED;
        }
    }

    template<>
    inline VkImageLayout convert(const RHI::RenderPassTargetType& value)
    {
        switch (value)
        {
        case RHI::RenderPassTargetType::Color: return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        case RHI::RenderPassTargetType::DepthStencil: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        default: return VK_IMAGE_LAYOUT_UNDEFINED;
        }
    }

    template<>
    inline VkColorComponentFlags convert(const ColorChannelFlags& value)
    {
        VkColorComponentFlags result = 0;
        if (value & ColorChannels::Red) result |= VK_COLOR_COMPONENT_R_BIT;
        if (value & ColorChannels::Green) result |= VK_COLOR_COMPONENT_G_BIT;
        if (value & ColorChannels::Blue) result |= VK_COLOR_COMPONENT_B_BIT;
        if (value & ColorChannels::Alpha) result |= VK_COLOR_COMPONENT_A_BIT;
        return result;
    }

    template<>
    inline VkImageSubresourceRange convert(const TextureSubresourceRange& value)
    {
        VkImageSubresourceRange range;
        range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        range.baseMipLevel = value.startMipIndex;
        range.levelCount = value.mipCount;
        range.baseArrayLayer = value.startArrayIndex;
        range.layerCount = value.layerCount;
        return range;
    }


    template<>
    inline Array<VkPushConstantRange> convert(const Array<ShaderPushConstantVariable>& variables)
    {
        Array<VkPushConstantRange> ranges;

        for (const auto& var : variables)
        {
            VkPushConstantRange range;
            range.offset = var.offset;
            range.size = var.size;
            range.stageFlags = convert<VkShaderStageFlags>(var.visibility);

            ranges.add(range);
        }

        return ranges;
    }


    template<>
    inline VkDescriptorSetLayoutBinding convert(const ShaderBinding& binding)
    {
        VkDescriptorSetLayoutBinding result{};
        result.binding = binding.bindingIndex;
        result.stageFlags = convert<VkShaderStageFlags>(binding.visibility);
        result.descriptorCount = binding.arrayCount;
        result.descriptorType = convert<VkDescriptorType>(binding.bindingType);
        return result;
    }


    template<>
    inline VkDescriptorType convert(const TextureBindingType& value)
    {
        switch (value)
        {
        case TextureBindingType::Storage: return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        case TextureBindingType::Sampled: return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        default: throw;
        }
    }

    template<>
    inline VkDescriptorType convert(const BufferBindingType& value)
    {
        switch (value)
        {
        case BufferBindingType::Uniform: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        case BufferBindingType::Storage: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        case BufferBindingType::UniformTexel: return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
        case BufferBindingType::StorageTexel: return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
        default: throw;
        }
    }
}
