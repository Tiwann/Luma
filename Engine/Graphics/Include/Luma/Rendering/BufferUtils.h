#pragma once
#include <cstdint>

namespace Luma
{
    struct IBuffer;
    struct IRenderDevice;

    namespace BufferUtils
    {
        IBuffer* createStagingBuffer(IRenderDevice* device, const void* data, uint64_t size);
        IBuffer* createVertexBuffer(IRenderDevice* device, const void* data, uint64_t size);
        IBuffer* createIndexBuffer(IRenderDevice* device, const void* data, uint64_t size);

        template<typename T, uint64_t N>
        IBuffer* createVertexBuffer(const IRenderDevice* device, const T(&data)[N])
        {
            return createVertexBuffer(device, data, sizeof(data[0]) * N);
        }

        template<typename T, uint64_t N>
        IBuffer* createIndexBuffer(const IRenderDevice* device, const T(&data)[N])
        {
            return createIndexBuffer(device, data, sizeof(data[0]) * N);
        }
    }
}