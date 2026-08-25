#pragma once
#include "Luma/Runtime/Format.h"
#include "Luma/Rendering/PresentMode.h"
#include "Luma/Rendering/ResourceState.h"
#include "Luma/Rendering/QueueType.h"
#include "Luma/Rendering/SamplerAddressMode.h"
#include "Luma/Rendering/CompareOperation.h"
#include "Luma/Rendering/BlendFactor.h"
#include "Luma/Rendering/BlendOperation.h"
#include "Luma/Rendering/PolygonMode.h"
#include "Luma/Rendering/PrimitiveTopology.h"
#include "Luma/Rendering/VertexInputRate.h"

#include <dxgi1_6.h>
#include <directx/d3dx12.h>

#include "Luma/Rendering/IndexFormat.h"

namespace Luma::D3D12
{
    template<typename OutType, typename InType>
    OutType convert(const InType& value);

    template <>
    inline DXGI_FORMAT convert(const EFormat& value)
    {
        switch (value)
        {
        case EFormat::None: return DXGI_FORMAT_UNKNOWN;

        case EFormat::R8_UNORM: return DXGI_FORMAT_R8_UNORM;
        case EFormat::R8_SNORM: return DXGI_FORMAT_R8_SNORM;
        case EFormat::R16_USHORT: return DXGI_FORMAT_R16_UNORM;
        case EFormat::R16_SHORT: return DXGI_FORMAT_R16_SNORM;
        case EFormat::R32_FLOAT: return DXGI_FORMAT_R32_FLOAT;
        case EFormat::R32_UINT: return DXGI_FORMAT_R32_UINT;
        case EFormat::R32_SINT: return DXGI_FORMAT_R32_SINT;

        case EFormat::R8G8_UNORM: return DXGI_FORMAT_R8G8_UNORM;
        case EFormat::R8G8_SNORM: return DXGI_FORMAT_R8G8_SNORM;
        case EFormat::R16G16_USHORT: return DXGI_FORMAT_R16G16_UNORM;
        case EFormat::R16G16_SHORT: return DXGI_FORMAT_R16G16_SNORM;
        case EFormat::R32G32_UINT: return DXGI_FORMAT_R32G32_UINT;
        case EFormat::R32G32_SINT: return DXGI_FORMAT_R32G32_SINT;
        case EFormat::R32G32_FLOAT: return DXGI_FORMAT_R32G32_FLOAT;

        case EFormat::R8G8B8_UNORM: return DXGI_FORMAT_UNKNOWN;
        case EFormat::R8G8B8_SNORM: return DXGI_FORMAT_UNKNOWN;
        case EFormat::R16G16B16_USHORT: return DXGI_FORMAT_UNKNOWN;
        case EFormat::R16G16B16_SHORT: return DXGI_FORMAT_UNKNOWN;
        case EFormat::R32G32B32_UINT: return DXGI_FORMAT_R32G32B32_UINT;
        case EFormat::R32G32B32_SINT: return DXGI_FORMAT_R32G32B32_SINT;
        case EFormat::R32G32B32_FLOAT: return DXGI_FORMAT_R32G32B32_FLOAT;

        case EFormat::R8G8B8A8_UNORM: return DXGI_FORMAT_R8G8B8A8_UNORM;
        case EFormat::R8G8B8A8_SNORM: return DXGI_FORMAT_R8G8B8A8_SNORM;
        case EFormat::R8G8B8A8_SRGB: return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        case EFormat::R16G16B16A16_USHORT: return DXGI_FORMAT_R16G16B16A16_UNORM;
        case EFormat::R16G16B16A16_SHORT: return DXGI_FORMAT_R16G16B16A16_SNORM;
        case EFormat::R32G32B32A32_UINT: return DXGI_FORMAT_R32G32B32A32_UINT;
        case EFormat::R32G32B32A32_SINT: return DXGI_FORMAT_R32G32B32A32_SINT;
        case EFormat::R32G32B32A32_FLOAT: return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case EFormat::R64G64B64A64_FLOAT: return DXGI_FORMAT_UNKNOWN;

        case EFormat::D32_FLOAT: return DXGI_FORMAT_D32_FLOAT;
        case EFormat::D32_FLOAT_S8_UINT: return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        case EFormat::D24_UNORM_S8_UINT: return DXGI_FORMAT_D24_UNORM_S8_UINT;

        case EFormat::ASTC_4X4_UNORM:
        case EFormat::ASTC_4X4_FLOAT:
        case EFormat::ASTC_4X4_SRGB:
        case EFormat::ASTC_5X4_UNORM:
        case EFormat::ASTC_5X4_FLOAT:
        case EFormat::ASTC_5X4_SRGB:
        case EFormat::ASTC_5X5_UNORM:
        case EFormat::ASTC_5X5_FLOAT:
        case EFormat::ASTC_5X5_SRGB:
        case EFormat::ASTC_6X5_UNORM:
        case EFormat::ASTC_6X5_FLOAT:
        case EFormat::ASTC_6X5_SRGB:
        case EFormat::ASTC_6X6_UNORM:
        case EFormat::ASTC_6X6_FLOAT:
        case EFormat::ASTC_6X6_SRGB:
        case EFormat::ASTC_8X5_UNORM:
        case EFormat::ASTC_8X5_FLOAT:
        case EFormat::ASTC_8X5_SRGB:
        case EFormat::ASTC_8X6_UNORM:
        case EFormat::ASTC_8X6_FLOAT:
        case EFormat::ASTC_8X6_SRGB:
        case EFormat::ASTC_8X8_UNORM:
        case EFormat::ASTC_8X8_FLOAT:
        case EFormat::ASTC_8X8_SRGB:
        case EFormat::ASTC_10X5_UNORM:
        case EFormat::ASTC_10X5_FLOAT:
        case EFormat::ASTC_10X5_SRGB:
        case EFormat::ASTC_10X6_UNORM:
        case EFormat::ASTC_10X6_FLOAT:
        case EFormat::ASTC_10X6_SRGB:
        case EFormat::ASTC_10X8_UNORM:
        case EFormat::ASTC_10X8_FLOAT:
        case EFormat::ASTC_10X8_SRGB:
        case EFormat::ASTC_10X10_UNORM:
        case EFormat::ASTC_10X10_FLOAT:
        case EFormat::ASTC_10X10_SRGB:
        case EFormat::ASTC_12X10_UNORM:
        case EFormat::ASTC_12X10_FLOAT:
        case EFormat::ASTC_12X10_SRGB:
        case EFormat::ASTC_12X12_UNORM:
        case EFormat::ASTC_12X12_FLOAT:
        case EFormat::ASTC_12X12_SRGB: return DXGI_FORMAT_UNKNOWN;

        case EFormat::BC1_UNORM: return DXGI_FORMAT_BC1_UNORM;
        case EFormat::BC1_SRGB: return DXGI_FORMAT_BC1_UNORM_SRGB;
        case EFormat::BC2_UNORM: return DXGI_FORMAT_BC2_UNORM;
        case EFormat::BC2_SRGB: return DXGI_FORMAT_BC2_UNORM_SRGB;
        case EFormat::BC3_UNORM: return DXGI_FORMAT_BC3_UNORM;
        case EFormat::BC3_SRGB: return DXGI_FORMAT_BC3_UNORM_SRGB;
        case EFormat::BC4_UNORM: return DXGI_FORMAT_BC4_UNORM;
        case EFormat::BC4_SNORM: return DXGI_FORMAT_BC4_SNORM;
        case EFormat::BC5_UNORM: return DXGI_FORMAT_BC5_UNORM;
        case EFormat::BC5_SNORM: return DXGI_FORMAT_BC5_SNORM;
        case EFormat::BC7_UNORM: return DXGI_FORMAT_BC7_UNORM;
        case EFormat::BC7_SRGB: return DXGI_FORMAT_BC7_UNORM_SRGB;
        }

        return DXGI_FORMAT_UNKNOWN;
    }

    template <>
    inline DXGI_SWAP_EFFECT convert(const EPresentMode& value)
    {
        switch (value)
        {
        case EPresentMode::Unknown: throw;
        case EPresentMode::Immediate: return DXGI_SWAP_EFFECT_DISCARD;
        case EPresentMode::Mailbox: return DXGI_SWAP_EFFECT_FLIP_DISCARD;
        case EPresentMode::Fifo: return DXGI_SWAP_EFFECT_FLIP_DISCARD;
        default: throw;
        }
    }

    template<>
    inline D3D12_RESOURCE_STATES convert(const EResourceState& resourceState)
    {
        switch (resourceState)
        {
        case EResourceState::Undefined: return D3D12_RESOURCE_STATE_COMMON;
        case EResourceState::General: return D3D12_RESOURCE_STATE_COMMON;
        case EResourceState::ShaderRead: return D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE;
        case EResourceState::ShaderWrite: return D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
        case EResourceState::ColorAttachment: return D3D12_RESOURCE_STATE_RENDER_TARGET;
        case EResourceState::DepthStencilAttachment: return D3D12_RESOURCE_STATE_DEPTH_WRITE;
        case EResourceState::CopySource: return D3D12_RESOURCE_STATE_COPY_SOURCE;
        case EResourceState::CopyDest: return D3D12_RESOURCE_STATE_COPY_DEST;
        case EResourceState::Present: return D3D12_RESOURCE_STATE_PRESENT;
        default: return D3D12_RESOURCE_STATE_COMMON;
        }
    }

    template<>
    inline EResourceState convert(const D3D12_RESOURCE_STATES& resourceState)
    {
        return EResourceState::Undefined;
    }

    template<>
    inline D3D12_COMMAND_LIST_TYPE convert(const EQueueType& queueType)
    {
        switch (queueType)
        {
        case EQueueType::None: return D3D12_COMMAND_LIST_TYPE_NONE;
        case EQueueType::Render: return D3D12_COMMAND_LIST_TYPE_DIRECT;
        case EQueueType::Compute: return D3D12_COMMAND_LIST_TYPE_COMPUTE;
        case EQueueType::Copy: return D3D12_COMMAND_LIST_TYPE_COPY;
        default: return D3D12_COMMAND_LIST_TYPE_NONE;
        }
    }

    template<>
    inline EQueueType convert(const D3D12_COMMAND_LIST_TYPE& listType)
    {
        switch (listType)
        {
        case D3D12_COMMAND_LIST_TYPE_NONE: return EQueueType::None;
        case D3D12_COMMAND_LIST_TYPE_DIRECT: return EQueueType::Render;
        case D3D12_COMMAND_LIST_TYPE_COMPUTE: return EQueueType::Compute;
        case D3D12_COMMAND_LIST_TYPE_COPY: return EQueueType::Copy;
        default: return EQueueType::None;
        }
    }

    template<>
    inline D3D12_INPUT_CLASSIFICATION convert(const EVertexInputRate& rate)
    {
        switch (rate)
        {
        case EVertexInputRate::Vertex: return D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        case EVertexInputRate::Instance: return D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
        default: return D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        }
    }

    template<>
    inline D3D12_PRIMITIVE_TOPOLOGY_TYPE convert(const EPrimitiveTopology& value)
    {
        switch (value)
        {
        case EPrimitiveTopology::PointList: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
        case EPrimitiveTopology::LineList: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
        case EPrimitiveTopology::LineStrip: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
        case EPrimitiveTopology::TriangleList: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        case EPrimitiveTopology::TriangleStrip: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        case EPrimitiveTopology::TriangleFan: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        default: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        }
    }

    template <>
    inline D3D12_BLEND convert(const EBlendFactor& factor)
    {
        switch (factor)
        {
        case EBlendFactor::Zero: return D3D12_BLEND_ZERO;
        case EBlendFactor::One: return D3D12_BLEND_ONE;
        case EBlendFactor::SourceColor: return D3D12_BLEND_SRC_COLOR;
        case EBlendFactor::OneMinusSourceColor: return D3D12_BLEND_INV_SRC_COLOR;
        case EBlendFactor::DestColor: return D3D12_BLEND_DEST_COLOR;
        case EBlendFactor::OneMinusDestColor: return D3D12_BLEND_INV_DEST_COLOR;
        case EBlendFactor::SourceAlpha: return D3D12_BLEND_SRC_ALPHA;
        case EBlendFactor::OneMinusSourceAlpha: return D3D12_BLEND_INV_SRC_ALPHA;
        case EBlendFactor::DestAlpha: return D3D12_BLEND_DEST_ALPHA;
        case EBlendFactor::OneMinusDestAlpha: return D3D12_BLEND_INV_DEST_ALPHA;
        case EBlendFactor::ConstantColor: return D3D12_BLEND_BLEND_FACTOR;
        case EBlendFactor::OneMinusConstantColor: return D3D12_BLEND_INV_BLEND_FACTOR;
        case EBlendFactor::ConstantAlpha: return D3D12_BLEND_BLEND_FACTOR;
        case EBlendFactor::OneMinusConstantAlpha: return D3D12_BLEND_INV_BLEND_FACTOR;
        case EBlendFactor::SourceAlphaSaturated: return D3D12_BLEND_SRC_ALPHA_SAT;
        case EBlendFactor::Source1Color: return D3D12_BLEND_SRC1_COLOR;
        case EBlendFactor::OneMinusSource1Color: return D3D12_BLEND_INV_SRC1_COLOR;
        case EBlendFactor::Source1Alpha: return D3D12_BLEND_SRC1_ALPHA;
        case EBlendFactor::OneMinusSource1Alpha: return D3D12_BLEND_INV_SRC1_ALPHA;
        default: return D3D12_BLEND_ZERO;
        }
    }

    template <>
    inline D3D12_BLEND_OP convert(const EBlendOp& op)
    {
        switch (op)
        {
        case EBlendOp::Add: return D3D12_BLEND_OP_ADD;
        case EBlendOp::Min: return D3D12_BLEND_OP_MIN;
        case EBlendOp::Max: return D3D12_BLEND_OP_MAX;
        case EBlendOp::Subtract: return D3D12_BLEND_OP_SUBTRACT;
        case EBlendOp::ReverseSubtract: return D3D12_BLEND_OP_REV_SUBTRACT;
        default: return D3D12_BLEND_OP_ADD;
        }
    }

    template <>
    inline D3D12_COMPARISON_FUNC convert(const ECompareOp& op)
    {
        switch (op)
        {
        case ECompareOp::Always: return D3D12_COMPARISON_FUNC_ALWAYS;
        case ECompareOp::Never: return D3D12_COMPARISON_FUNC_NEVER;
        case ECompareOp::Less: return D3D12_COMPARISON_FUNC_LESS;
        case ECompareOp::LessOrEqual: return D3D12_COMPARISON_FUNC_LESS_EQUAL;
        case ECompareOp::Equal: return D3D12_COMPARISON_FUNC_EQUAL;
        case ECompareOp::NotEqual: return D3D12_COMPARISON_FUNC_NOT_EQUAL;
        case ECompareOp::Greater: return D3D12_COMPARISON_FUNC_GREATER;
        case ECompareOp::GreaterOrEqual: return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
        default: return D3D12_COMPARISON_FUNC_ALWAYS;
        }
    }

    template <>
    inline D3D12_TEXTURE_ADDRESS_MODE convert(const ESamplerAddressMode& mode)
    {
        switch (mode)
        {
        case ESamplerAddressMode::Repeat: return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        case ESamplerAddressMode::MirroredRepeat: return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
        case ESamplerAddressMode::ClampToEdge: return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        case ESamplerAddressMode::ClampToBorder:return D3D12_TEXTURE_ADDRESS_MODE_BORDER;
        case ESamplerAddressMode::MirrorClampToEdge:return D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE;
        default: return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        }
    }

    template <>
    inline DXGI_FORMAT convert(const EIndexFormat& fmt)
    {
        switch (fmt)
        {
        case EIndexFormat::UInt32: return DXGI_FORMAT_R32_UINT;
        case EIndexFormat::UInt16: return DXGI_FORMAT_R16_UINT;
        case EIndexFormat::UInt8: return DXGI_FORMAT_R8_UINT;
        default: throw;
        }
    }
}
