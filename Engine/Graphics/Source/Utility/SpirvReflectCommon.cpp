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
        case SPV_REFLECT_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: return EBindingType::InputTarget;
        case SPV_REFLECT_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR: return EBindingType::AccelerationStructure;
        }
        return (EBindingType)0;
    };

    EShaderStage getShaderStage(SpvReflectShaderStageFlagBits bits)
    {
        switch (bits)
        {
        case SPV_REFLECT_SHADER_STAGE_VERTEX_BIT: return EShaderStage::Vertex;
        case SPV_REFLECT_SHADER_STAGE_TESSELLATION_CONTROL_BIT: return EShaderStage::TessellationControl;
        case SPV_REFLECT_SHADER_STAGE_TESSELLATION_EVALUATION_BIT: return EShaderStage::TessellationEvaluation;
        case SPV_REFLECT_SHADER_STAGE_GEOMETRY_BIT: return EShaderStage::Geometry;
        case SPV_REFLECT_SHADER_STAGE_FRAGMENT_BIT: return EShaderStage::Fragment;
        case SPV_REFLECT_SHADER_STAGE_COMPUTE_BIT: return EShaderStage::Compute;
        case SPV_REFLECT_SHADER_STAGE_TASK_BIT_NV: return EShaderStage::Task;
        case SPV_REFLECT_SHADER_STAGE_MESH_BIT_NV: return EShaderStage::Mesh;
        case SPV_REFLECT_SHADER_STAGE_RAYGEN_BIT_KHR: return EShaderStage::None;
        case SPV_REFLECT_SHADER_STAGE_ANY_HIT_BIT_KHR: return EShaderStage::None;
        case SPV_REFLECT_SHADER_STAGE_CLOSEST_HIT_BIT_KHR: return EShaderStage::None;
        case SPV_REFLECT_SHADER_STAGE_MISS_BIT_KHR: return EShaderStage::None;
        case SPV_REFLECT_SHADER_STAGE_INTERSECTION_BIT_KHR: return EShaderStage::None;
        case SPV_REFLECT_SHADER_STAGE_CALLABLE_BIT_KHR: return EShaderStage::None;
        default: return EShaderStage::None;
        }
    };
}