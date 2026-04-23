#include "Luma/Rendering/GraphicsState.h"

namespace Luma
{
    void FGraphicsState::setGraphicsPipeline(const IGraphicsPipeline* pipeline)
    {
        m_GraphicsPipeline = pipeline;
    }

    void FGraphicsState::setVertexBuffer(const IBuffer* vertexBuffer)
    {
        m_VertexBuffer = vertexBuffer;
    }

    void FGraphicsState::setIndexBuffer(const IBuffer* indexBuffer)
    {
        m_IndexBuffer = indexBuffer;
    }

    void FGraphicsState::setIndexFormat(const EIndexFormat format)
    {
        m_IndexFormat = format;
    }

    void FGraphicsState::setViewport(const FViewport& viewport)
    {
        m_Viewport = viewport;
    }

    void FGraphicsState::setScissor(const FScissor& scissor)
    {
        m_Scissor = scissor;
    }

    const IGraphicsPipeline* FGraphicsState::getGraphicsPipeline() const
    {
        return m_GraphicsPipeline;
    }

    const IBuffer* FGraphicsState::getVertexBuffer() const
    {
        return m_VertexBuffer;
    }

    const IBuffer* FGraphicsState::getIndexBuffer() const
    {
        return m_IndexBuffer;
    }

    EIndexFormat FGraphicsState::getIndexFormat() const
    {
        return m_IndexFormat;
    }

    const FViewport& FGraphicsState::getViewport() const
    {
        return m_Viewport;
    }

    const FScissor& FGraphicsState::getScissor() const
    {
        return m_Scissor;
    }
}
