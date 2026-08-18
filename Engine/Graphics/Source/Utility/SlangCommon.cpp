#include "Luma/Utility/SlangCommon.h"
#include <slang/slang.h>

namespace Luma
{
    SlangCompileTarget getCompileTarget(const EShaderCompileTarget target)
    {
        switch (target)
        {
        case EShaderCompileTarget::SPIRV: return SLANG_SPIRV;
        case EShaderCompileTarget::GLSL: return SLANG_GLSL;
        case EShaderCompileTarget::HLSL: return SLANG_HLSL;
        case EShaderCompileTarget::DXBC: return SLANG_DXBC;
        case EShaderCompileTarget::DXIL: return SLANG_DXIL;
        case EShaderCompileTarget::Auto: return SLANG_SPIRV;
        default: return SLANG_SPIRV;
        }
    }

    EShaderStageBits getStage(const SlangStage stage)
    {
        switch (stage)
        {
        case SLANG_STAGE_NONE: return EShaderStageBits::None;
        case SLANG_STAGE_VERTEX: return EShaderStageBits::Vertex;
        case SLANG_STAGE_GEOMETRY: return EShaderStageBits::Geometry;
        case SLANG_STAGE_FRAGMENT: return EShaderStageBits::Fragment;
        case SLANG_STAGE_COMPUTE: return EShaderStageBits::Compute;
        case SLANG_STAGE_RAY_GENERATION: return EShaderStageBits::RayGeneration;
        case SLANG_STAGE_HULL: return EShaderStageBits::TessellationControl;
        case SLANG_STAGE_MESH: return EShaderStageBits::Mesh;
        default: return EShaderStageBits::None;
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

    slang::PreprocessorMacroDesc toPreprocessorMacroDesc(const FShaderCompileDefine& define)
    {
        slang::PreprocessorMacroDesc desc;
        desc.name = *define.key;
        desc.value = *define.value;
        return desc;
    }
}
