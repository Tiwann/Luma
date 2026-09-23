#pragma once
#include <cstdint>

namespace Luma
{
    namespace RHI
    {
        struct Device;
        struct Buffer;
    }

    namespace BufferUtils
    {
        RHI::Buffer* createStagingBuffer(RHI::Device* device, const void* data, uint64_t size);
        RHI::Buffer* createVertexBuffer(RHI::Device* device, const void* data, uint64_t size);
        RHI::Buffer* createIndexBuffer(RHI::Device* device, const void* data, uint64_t size);

        template<typename T, uint64_t N>
        RHI::Buffer* createVertexBuffer(const RHI::Device* device, const T(&data)[N])
        {
            return createVertexBuffer(device, data, sizeof(data[0]) * N);
        }

        template<typename T, uint64_t N>
        RHI::Buffer* createIndexBuffer(const RHI::Device* device, const T(&data)[N])
        {
            return createIndexBuffer(device, data, sizeof(data[0]) * N);
        }
    }
}