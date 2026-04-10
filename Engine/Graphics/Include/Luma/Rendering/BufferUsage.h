#pragma once

namespace luma
{
    enum class EBufferUsage
    {
        None,
        VertexBuffer,
        IndexBuffer,
        UniformBuffer,
        StorageBuffer,
        StagingBuffer,
        IndirectBuffer
    };
}
