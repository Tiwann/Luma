#pragma once

namespace Luma
{
    enum class EBindingType
    {
        Sampler,
        CombinedTextureSampler,
        SampledTexture,
        StorageTexture,
        UniformBuffer,
        StorageBuffer,
        InputAttachment,
        InlineUniformBlock,
        AccelerationStructure,
        StorageTexelBuffer,
        UniformTexelBuffer,
        PushConstant
    };
}