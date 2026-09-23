#include "Luma/Utility/SpirvReflectCommon.h"

namespace Luma
{
    BindingType getBindingType(const SpvReflectDescriptorType type)
    {
        switch (type)
        {
        case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLER: return BindingType::Sampler;
        case SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: return BindingType::CombinedTextureSampler;
        case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE: return BindingType::SampledTexture;
        case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE: return BindingType::StorageTexture;
        case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER: return BindingType::UniformTexelBuffer;
        case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER: return BindingType::StorageTexelBuffer;
        case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER: return BindingType::UniformBuffer;
        case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER: return BindingType::StorageBuffer;
        case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC: return BindingType::InlineUniformBlock;
        case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: return BindingType::InlineUniformBlock;
        case SPV_REFLECT_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: return BindingType::InputTarget;
        case SPV_REFLECT_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR: return BindingType::AccelerationStructure;
        }
        return (BindingType)0;
    };

    ShaderStage getShaderStage(SpvReflectShaderStageFlagBits bits)
    {
        switch (bits)
        {
        case SPV_REFLECT_SHADER_STAGE_VERTEX_BIT: return ShaderStage::Vertex;
        case SPV_REFLECT_SHADER_STAGE_TESSELLATION_CONTROL_BIT: return ShaderStage::TessellationControl;
        case SPV_REFLECT_SHADER_STAGE_TESSELLATION_EVALUATION_BIT: return ShaderStage::TessellationEvaluation;
        case SPV_REFLECT_SHADER_STAGE_GEOMETRY_BIT: return ShaderStage::Geometry;
        case SPV_REFLECT_SHADER_STAGE_FRAGMENT_BIT: return ShaderStage::Fragment;
        case SPV_REFLECT_SHADER_STAGE_COMPUTE_BIT: return ShaderStage::Compute;
        case SPV_REFLECT_SHADER_STAGE_TASK_BIT_NV: return ShaderStage::Task;
        case SPV_REFLECT_SHADER_STAGE_MESH_BIT_NV: return ShaderStage::Mesh;
        case SPV_REFLECT_SHADER_STAGE_RAYGEN_BIT_KHR: return ShaderStage::None;
        case SPV_REFLECT_SHADER_STAGE_ANY_HIT_BIT_KHR: return ShaderStage::None;
        case SPV_REFLECT_SHADER_STAGE_CLOSEST_HIT_BIT_KHR: return ShaderStage::None;
        case SPV_REFLECT_SHADER_STAGE_MISS_BIT_KHR: return ShaderStage::None;
        case SPV_REFLECT_SHADER_STAGE_INTERSECTION_BIT_KHR: return ShaderStage::None;
        case SPV_REFLECT_SHADER_STAGE_CALLABLE_BIT_KHR: return ShaderStage::None;
        default: return ShaderStage::None;
        }
    };
}