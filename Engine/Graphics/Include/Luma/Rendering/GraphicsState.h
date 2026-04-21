#pragma once

namespace Luma
{
    struct IGraphicsPipeline;
    struct IComputePipeline;
    struct IBuffer;

    struct FGraphicsState
    {
        IGraphicsPipeline* graphicsPipeline = nullptr;
        IComputePipeline* computePipeline = nullptr;
        IBuffer* vertexBuffer = nullptr;
        IBuffer* indexBuffer = nullptr;
    };
}