#pragma once
#include <cstdint>

namespace Luma
{
    struct IBuffer;
    struct IGPUDevice;

    namespace BufferUtils
    {
        IBuffer* createStagingBuffer(IGPUDevice* device, const void* data, uint64_t size);
        IBuffer* createVertexBuffer(IGPUDevice* device, const void* data, uint64_t size);
        IBuffer* createIndexBuffer(IGPUDevice* device, const void* data, uint64_t size);

        template<typename T, uint64_t N>
        IBuffer* createVertexBuffer(const IGPUDevice* device, const T(&data)[N])
        {
            return createVertexBuffer(device, data, sizeof(data[0]) * N);
        }

        template<typename T, uint64_t N>
        IBuffer* createIndexBuffer(const IGPUDevice* device, const T(&data)[N])
        {
            return createIndexBuffer(device, data, sizeof(data[0]) * N);
        }
    }
}