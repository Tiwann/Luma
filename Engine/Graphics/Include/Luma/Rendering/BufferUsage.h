#pragma once

namespace Luma
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
