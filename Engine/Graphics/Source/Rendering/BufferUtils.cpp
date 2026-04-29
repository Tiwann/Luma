#include "Luma/Rendering/BufferUtils.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Rendering/Buffer.h"
#include "Luma/Rendering/CommandBuffer.h"
#include "Luma/Rendering/Fence.h"
#include "Luma/Rendering/Queue.h"
#include "Luma/Rendering/RenderDevice.h"

namespace Luma::BufferUtils
{
    IBuffer* createStagingBuffer(IRenderDevice* device, const void* data, size_t size)
    {
        FBufferDesc bufferDesc;
        bufferDesc.size = size;
        bufferDesc.usage = EBufferUsage::StagingBuffer;
        IBuffer* stagingBuffer = device->createBuffer(bufferDesc);
        if (!stagingBuffer) return nullptr;

        void* mappedData = stagingBuffer->map();
        memcpy(mappedData, data, size);
        stagingBuffer->unmap(mappedData);
        return stagingBuffer;
    }

    IBuffer* createVertexBuffer(IRenderDevice* device, const void* data, const size_t size)
    {
        Ref<IBuffer> stagingBuffer = Ref(createStagingBuffer(device, data, size));
        if (!stagingBuffer) return nullptr;

        FBufferDesc bufferDesc;
        bufferDesc.size = size;
        bufferDesc.usage = EBufferUsage::VertexBuffer;
        IBuffer* vertexBuffer = device->createBuffer(bufferDesc);
        if (!vertexBuffer) return nullptr;

        Ref<ICommandBuffer> cmdBuffer = Ref(device->createCopyCommandBuffer());
        if (!cmdBuffer)
        {
            vertexBuffer->destroy();
            return nullptr;
        }

        if (cmdBuffer->begin())
        {
            cmdBuffer->copyBuffer(stagingBuffer, vertexBuffer, 0, 0, size);
            cmdBuffer->end();

            Ref<IFence> fence = Ref(device->createFence(FFenceDesc()));
            if (!fence)
            {
                vertexBuffer->destroy();
                return nullptr;
            }

            const IQueue* copyQueue = device->getCopyQueue();
            // copyQueue->
            fence->wait(FENCE_WAIT_INFINITE);
            return vertexBuffer;
        }

        vertexBuffer->destroy();
        return nullptr;
    }

    IBuffer* createIndexBuffer(IRenderDevice* device, const void* data, const size_t size)
    {
        Ref<IBuffer> stagingBuffer = Ref(createStagingBuffer(device, data, size));
        if (!stagingBuffer) return nullptr;

        FBufferDesc bufferDesc;
        bufferDesc.size = size;
        bufferDesc.usage = EBufferUsage::IndexBuffer;
        IBuffer* indexBuffer = device->createBuffer(bufferDesc);
        if (!indexBuffer) return nullptr;

        Ref<ICommandBuffer> cmdBuffer = Ref(device->createCopyCommandBuffer());
        if (!cmdBuffer)
        {
            indexBuffer->destroy();
            return nullptr;
        }

        if (cmdBuffer->begin())
        {
            cmdBuffer->copyBuffer(stagingBuffer, indexBuffer, 0, 0, size);
            cmdBuffer->end();

            Ref<IFence> fence = Ref(device->createFence(FFenceDesc()));
            if (!fence)
            {
                indexBuffer->destroy();
                return nullptr;
            }

            const IQueue* copyQueue = device->getCopyQueue();
            // copyQueue->
            fence->wait(FENCE_WAIT_INFINITE);
            return indexBuffer;
        }

        indexBuffer->destroy();
        return nullptr;
    }
}
