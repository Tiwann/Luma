#include "Luma/Utility/SpirvReflectCommon.h"

namespace Luma
{
    EBindingType getBindingType(const SpvReflectDescriptorType type)
    {
        switch (type)
        {
        case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLER: return EBindingType::Sampler;
        case SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: return EBindingType::CombinedTextureSampler;
        case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE: return EBindingType::SampledTexture;
        case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE: return EBindingType::StorageTexture;
        case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER: return EBindingType::UniformTexelBuffer;
        case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER: return EBindingType::StorageTexelBuffer;
        case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER: return EBindingType::UniformBuffer;
        case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER: return EBindingType::StorageBuffer;
        case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC: return EBindingType::InlineUniformBlock;
        case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: return EBindingType::InlineUniformBlock;
        case SPV_REFLECT_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: return EBindingType::InputAttachment;
        case SPV_REFLECT_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR: return EBindingType::AccelerationStructure;
        }
        return (EBindingType)0;
    };

    EShaderStageBits getShaderStage(SpvReflectShaderStageFlagBits bits)
    {
        switch (bits)
        {
        case SPV_REFLECT_SHADER_STAGE_VERTEX_BIT: return EShaderStageBits::Vertex;
        case SPV_REFLECT_SHADER_STAGE_TESSELLATION_CONTROL_BIT: return EShaderStageBits::TessellationControl;
        case SPV_REFLECT_SHADER_STAGE_TESSELLATION_EVALUATION_BIT: return EShaderStageBits::None;
        case SPV_REFLECT_SHADER_STAGE_GEOMETRY_BIT: return EShaderStageBits::Geometry;
        case SPV_REFLECT_SHADER_STAGE_FRAGMENT_BIT: return EShaderStageBits::Fragment;
        case SPV_REFLECT_SHADER_STAGE_COMPUTE_BIT: return EShaderStageBits::Compute;
        case SPV_REFLECT_SHADER_STAGE_TASK_BIT_NV: return EShaderStageBits::None;
        case SPV_REFLECT_SHADER_STAGE_MESH_BIT_NV: return EShaderStageBits::Mesh;
        case SPV_REFLECT_SHADER_STAGE_RAYGEN_BIT_KHR: return EShaderStageBits::RayGeneration;
        case SPV_REFLECT_SHADER_STAGE_ANY_HIT_BIT_KHR: return EShaderStageBits::None;
        case SPV_REFLECT_SHADER_STAGE_CLOSEST_HIT_BIT_KHR: return EShaderStageBits::None;
        case SPV_REFLECT_SHADER_STAGE_MISS_BIT_KHR: return EShaderStageBits::None;
        case SPV_REFLECT_SHADER_STAGE_INTERSECTION_BIT_KHR: return EShaderStageBits::None;
        case SPV_REFLECT_SHADER_STAGE_CALLABLE_BIT_KHR: return EShaderStageBits::None;
        default: return EShaderStageBits::None;
        }
    };
}