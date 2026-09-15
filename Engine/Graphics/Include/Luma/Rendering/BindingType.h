#pragma once
#include <cstdint>

namespace Luma
{
    enum class EBindingType : uint32_t
    {
        None,
        Sampler,
        CombinedTextureSampler,
        SampledTexture,
        StorageTexture,
        UniformBuffer,
        StorageBuffer,
        InputTarget,
        InlineUniformBlock,
        AccelerationStructure,
        StorageTexelBuffer,
        UniformTexelBuffer,
        PushConstant
    };
}