#pragma once

namespace Luma
{
    enum class BufferUsage
    {
        None,
        VertexBuffer,
        IndexBuffer,
        UniformBuffer,
        StorageBuffer,
        StagingBuffer,
        IndirectBuffer,
        DescriptorBuffer
    };
}
