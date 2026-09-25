#include "Luma/Rendering/BufferUtils.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Rendering/Buffer.h"
#include "Luma/Rendering/CommandBuffer.h"
#include "Luma/Rendering/Fence.h"
#include "Luma/Rendering/Queue.h"
#include "Luma/Rendering/Device.h"


namespace Luma::BufferUtils
{
    using namespace RHI;

    Buffer* createStagingBuffer(Device* device, const void* data, size_t size)
    {
        BufferDesc bufferDesc;
        bufferDesc.size = size;
        bufferDesc.usage = BufferUsage::StagingBuffer;
        Buffer* stagingBuffer = device->createBuffer(bufferDesc);
        if (!stagingBuffer) return nullptr;

        void* mappedData = stagingBuffer->map();
        memcpy(mappedData, data, size);
        stagingBuffer->unmap(mappedData);
        return stagingBuffer;
    }

    Buffer* createVertexBuffer(Device* device, const void* data, const size_t size)
    {
        Ref<Buffer> stagingBuffer = Ref(createStagingBuffer(device, data, size));
        if (!stagingBuffer) return nullptr;

        BufferDesc bufferDesc;
        bufferDesc.size = size;
        bufferDesc.usage = BufferUsage::VertexBuffer;
        Buffer* vertexBuffer = device->createBuffer(bufferDesc);
        if (!vertexBuffer) return nullptr;

        Queue* copyQueue = device->getCopyQueue();
        Ref<CommandBuffer> cmdBuffer = device->createCommandBuffer(copyQueue);

        if (cmdBuffer->begin())
        {
            cmdBuffer->copyBuffer(stagingBuffer, vertexBuffer, 0, 0, size);
            cmdBuffer->end();

            Ref<Fence> fence = Ref(device->createFence(0));

            FenceSync signal;
            signal.fence = fence;
            signal.value = 1;
            signal.stages = PipelineStages::Copy;

            QueueExecuteInfo execInfo;
            execInfo.cmdBuffers = {cmdBuffer};
            execInfo.signals = signal;

            copyQueue->executeCommandBuffers(execInfo);
            fence->waitOnCPU(1);

            return vertexBuffer;
        }

        vertexBuffer->destroy();
        return nullptr;
    }

    Buffer* createIndexBuffer(Device* device, const void* data, const size_t size)
    {
        Ref<Buffer> stagingBuffer = Ref(createStagingBuffer(device, data, size));
        if (!stagingBuffer) return nullptr;

        BufferDesc bufferDesc;
        bufferDesc.size = size;
        bufferDesc.usage = BufferUsage::IndexBuffer;
        Buffer* indexBuffer = device->createBuffer(bufferDesc);
        if (!indexBuffer) return nullptr;

        Queue* copyQueue = device->getCopyQueue();
        Ref<CommandBuffer> cmdBuffer = device->createCommandBuffer(copyQueue);
        LUMA_ASSERT(cmdBuffer, "Failed to create command buffer! Maybe pool is full ?");

        if (cmdBuffer->begin())
        {
            cmdBuffer->copyBuffer(stagingBuffer, indexBuffer, 0, 0, size);
            cmdBuffer->end();

            Ref<Fence> fence = Ref(device->createFence(0));

            FenceSync signal;
            signal.fence = fence;
            signal.value = 1;
            signal.stages = PipelineStages::Copy;

            QueueExecuteInfo execInfo;
            execInfo.cmdBuffers = {cmdBuffer};
            execInfo.signals = signal;

            copyQueue->executeCommandBuffers(execInfo);
            fence->waitOnCPU(1);
            return indexBuffer;
        }

        indexBuffer->destroy();
        return nullptr;
    }
}
