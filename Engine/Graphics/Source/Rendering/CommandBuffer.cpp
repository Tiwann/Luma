#include "Luma/Rendering/CommandBuffer.h"

namespace Luma
{
    void ICommandBuffer::draw(const uint32_t vertexCount, const uint32_t instanceCount,
                                    const uint32_t firstVertex, const uint32_t firstInstance)
    {
        const FDrawCommand drawCmd{vertexCount, instanceCount, firstVertex, firstInstance};
        draw(drawCmd);
    }

    void ICommandBuffer::drawIndexed(const uint32_t indexCount, const uint32_t instanceCount, const uint32_t firstIndex,
                                           const int32_t vertexOffset, const uint32_t firstInstance)
    {
        const FDrawIndexedCommand drawIndexedCmd{indexCount, instanceCount, firstIndex, vertexOffset, firstInstance};
        drawIndexed(drawIndexedCmd);
    }

    void FRenderCommandBuffer::bindVertexBuffer(const IBuffer* buffer, const int64_t offset)
    {
        m_CmdBuffer->bindVertexBuffer(buffer, offset);
    }

    void FRenderCommandBuffer::bindIndexBuffer(const IBuffer* buffer, const uint64_t offset, const EIndexFormat format)
    {
        m_CmdBuffer->bindIndexBuffer(buffer, offset, format);
    }

    void FRenderCommandBuffer::bindGraphicsPipeline(const IGraphicsPipeline* pipeline)
    {
        m_CmdBuffer->bindGraphicsPipeline(pipeline);
    }

    void FRenderCommandBuffer::setScissor(const FRect2u& scissor)
    {
        m_CmdBuffer->setScissor(scissor);
    }

    void FRenderCommandBuffer::setViewport(const FRect2f& viewport, const float minDepth, const float maxDepth)
    {
        m_CmdBuffer->setViewport(viewport, minDepth, maxDepth);
    }

    void FRenderCommandBuffer::draw(const FDrawCommand& drawCmd)
    {
        m_CmdBuffer->draw(drawCmd);
    }

    void FRenderCommandBuffer::drawIndexed(const FDrawIndexedCommand& drawIndexedCmd)
    {
        m_CmdBuffer->drawIndexed(drawIndexedCmd);
    }

    void FRenderCommandBuffer::drawIndirect(const IBuffer* buffer, const uint64_t offset, const uint32_t drawCount)
    {
        m_CmdBuffer->drawIndirect(buffer, offset, drawCount);
    }

    void FRenderCommandBuffer::drawIndexedIndirect(const IBuffer* buffer, const uint64_t offset, const uint32_t drawCount)
    {
        m_CmdBuffer->drawIndexedIndirect(buffer, offset, drawCount);
    }
}
