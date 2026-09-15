#include "Luma/Utility/SlangCommon.h"
#include <slang/slang.h>

namespace Luma
{
    EShaderStages getStage(const SlangStage stage)
    {
        switch (stage)
        {
        case SLANG_STAGE_NONE: return EShaderStages::None;
        case SLANG_STAGE_VERTEX: return EShaderStages::Vertex;
        case SLANG_STAGE_GEOMETRY: return EShaderStages::Geometry;
        case SLANG_STAGE_FRAGMENT: return EShaderStages::Fragment;
        case SLANG_STAGE_COMPUTE: return EShaderStages::Compute;
        case SLANG_STAGE_RAY_GENERATION: return EShaderStages::RayGeneration;
        case SLANG_STAGE_HULL: return EShaderStages::TessellationControl;
        case SLANG_STAGE_MESH: return EShaderStages::Mesh;
        default: return EShaderStages::None;
        }
    }

    EBindingType getBindingType(const slang::BindingType bindingType)
    {
        switch (bindingType)
        {
        case slang::BindingType::Sampler: return EBindingType::Sampler;
        case slang::BindingType::Texture: return EBindingType::SampledTexture;
        case slang::BindingType::ConstantBuffer:
        case slang::BindingType::ParameterBlock: return EBindingType::UniformBuffer;
        case slang::BindingType::TypedBuffer: return EBindingType::UniformTexelBuffer;
        case slang::BindingType::RawBuffer: return EBindingType::UniformBuffer;
        case slang::BindingType::CombinedTextureSampler: return EBindingType::CombinedTextureSampler;
        case slang::BindingType::InputRenderTarget: return EBindingType::InputAttachment;
        case slang::BindingType::InlineUniformData: return EBindingType::InlineUniformBlock;
        case slang::BindingType::RayTracingAccelerationStructure: return EBindingType::AccelerationStructure;
        case slang::BindingType::PushConstant: return EBindingType::PushConstant;
        case slang::BindingType::MutableTexture: return EBindingType::StorageTexture;
        case slang::BindingType::MutableTypedBuffer: return EBindingType::StorageBuffer;
        case slang::BindingType::MutableRawBuffer: return EBindingType::StorageBuffer;
        default: break;
        }
        return EBindingType::PushConstant;
    }
}
