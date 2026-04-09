#pragma once
#include "Interface.h"
#include <cstdint>


namespace luma
{
    struct IRenderDevice;
    struct IBuffer;
    struct IGraphicsPipeline;
    struct ITexture;

    enum class ECommandBufferType : uint32_t
    {
        Render,
        Compute,
        Copy
    };

    struct FCommandBufferDesc
    {
        IRenderDevice* device = nullptr;
        ECommandBufferType cmdBufferType = ECommandBufferType::Render;
    };

    struct ICommandBuffer : TBase<FCommandBufferDesc>
    {
        ICommandBuffer() = default;
        ~ICommandBuffer() override;
        virtual ECommandBufferType getCommandBufferType() = 0;
    };

    class FRenderCommandBuffer : ICommandBuffer
    {
    public:
        FRenderCommandBuffer() = default;
        ECommandBufferType getCommandBufferType() override { return ECommandBufferType::Render; }
        virtual void bindVertexBuffer(IBuffer* buffer, int64_t offset) = 0;
        virtual void bindGraphicsPipeline(IGraphicsPipeline* pipeline) = 0;
    };

    class FComputeCommandBuffer : ICommandBuffer
    {
    public:
        FComputeCommandBuffer() = default;
        ECommandBufferType getCommandBufferType() override { return ECommandBufferType::Compute; }
        virtual void dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) = 0;
        virtual void dispatchIndirect(IBuffer* buffer, int64_t offset) = 0;
    };

    class FCopyCommandBuffer : ICommandBuffer
    {
        public:
        FCopyCommandBuffer() = default;
        ECommandBufferType getCommandBufferType() override { return ECommandBufferType::Copy; }
        virtual void copyBuffer(IBuffer* srcBuffer, int64_t srcOffset, uint64_t srcSize, IBuffer* dstBuffer, int64_t dstOffset, uint64_t dstSize) = 0;
        virtual void copyBufferToTexture(IBuffer* buffer, int64_t offset, uint64_t size, ITexture* texture, uint32_t arraySlice, uint32_t mipLevel) = 0;
    };
}
