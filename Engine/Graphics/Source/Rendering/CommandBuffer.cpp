#include "Luma/Rendering/CommandBuffer.h"

namespace Luma
{
    void ICommandBuffer::draw(const uint32_t vertexCount, const uint32_t instanceCount, const uint32_t firstVertex, const uint32_t firstInstance)
    {
        const FDrawCommand drawCmd{vertexCount, instanceCount, firstVertex, firstInstance};
        draw(drawCmd);
    }

    void ICommandBuffer::drawIndexed(const uint32_t indexCount, const uint32_t instanceCount, const uint32_t firstIndex, const int32_t vertexOffset, const uint32_t firstInstance)
    {
        const FDrawIndexedCommand drawIndexedCmd{indexCount, instanceCount, firstIndex, vertexOffset, firstInstance};
        drawIndexed(drawIndexedCmd);
    }
}
