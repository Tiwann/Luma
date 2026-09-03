#pragma once
#include <cstdint>

namespace Luma
{
    struct IBuffer;
    struct IGpuDevice;

    namespace BufferUtils
    {
        IBuffer* createStagingBuffer(IGpuDevice* device, const void* data, uint64_t size);
        IBuffer* createVertexBuffer(IGpuDevice* device, const void* data, uint64_t size);
        IBuffer* createIndexBuffer(IGpuDevice* device, const void* data, uint64_t size);

        template<typename T, uint64_t N>
        IBuffer* createVertexBuffer(const IGpuDevice* device, const T(&data)[N])
        {
            return createVertexBuffer(device, data, sizeof(data[0]) * N);
        }

        template<typename T, uint64_t N>
        IBuffer* createIndexBuffer(const IGpuDevice* device, const T(&data)[N])
        {
            return createIndexBuffer(device, data, sizeof(data[0]) * N);
        }
    }
}