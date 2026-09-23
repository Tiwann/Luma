#include "Luma/Rendering/CommandBuffer.h"

namespace Luma::RHI
{
    void CommandBuffer::bindVertexBuffer(const Buffer* buffer, int64_t offset)
    {
        const VertexBufferBinding binding{buffer, offset};
        bindVertexBuffers({binding});
    }

    void CommandBuffer::draw(const uint32_t vertexCount, const uint32_t instanceCount, const uint32_t firstVertex, const uint32_t firstInstance)
    {
        const DrawCommand drawCmd{vertexCount, instanceCount, firstVertex, firstInstance};
        draw(drawCmd);
    }

    void CommandBuffer::drawIndexed(const uint32_t indexCount, const uint32_t instanceCount, const uint32_t firstIndex, const int32_t vertexOffset, const uint32_t firstInstance)
    {
        const DrawIndexedCommand drawIndexedCmd{indexCount, instanceCount, firstIndex, vertexOffset, firstInstance};
        drawIndexed(drawIndexedCmd);
    }
}
