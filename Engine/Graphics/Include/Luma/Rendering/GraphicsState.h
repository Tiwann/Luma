#pragma once
#include "IndexFormat.h"
#include "Viewport.h"
#include "Scissor.h"

namespace Luma
{
    struct IGraphicsPipeline;
    struct IComputePipeline;
    struct IBuffer;
    struct ITextureView;

    class FGraphicsState final
    {
    public:
        void setGraphicsPipeline(const IGraphicsPipeline* pipeline);
        void setVertexBuffer(const IBuffer* vertexBuffer);
        void setIndexBuffer(const IBuffer* indexBuffer);
        void setIndexFormat(EIndexFormat format);
        void setViewport(const FViewport& viewport);
        void setScissor(const FScissor& scissor);

        const IGraphicsPipeline* getGraphicsPipeline() const;
        const IBuffer* getVertexBuffer() const;
        const IBuffer* getIndexBuffer() const;
        EIndexFormat getIndexFormat() const;
        const FViewport& getViewport() const;
        const FScissor& getScissor() const;
    private:
        const IGraphicsPipeline* m_GraphicsPipeline = nullptr;
        const IBuffer* m_VertexBuffer = nullptr;
        const IBuffer* m_IndexBuffer = nullptr;
        EIndexFormat m_IndexFormat = EIndexFormat::Uint32;
        FViewport m_Viewport;
        FScissor m_Scissor;
    };
}
