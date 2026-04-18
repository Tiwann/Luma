#pragma once

namespace Luma
{
    enum class EDescriptorType
    {
        None,
        UniformBuffer,
        StorageBuffer,
        SampledImage,
        StorageImage,
        Sampler
    };
}