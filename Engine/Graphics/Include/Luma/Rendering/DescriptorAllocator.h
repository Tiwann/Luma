#pragma once
#include "DescriptorType.h"
#include <cstdint>

namespace Luma
{
    struct IBuffer;

    struct FDescriptorHandle
    {
        uint32_t index = 0;
        EDescriptorType type = EDescriptorType::None;
    };

    struct FDescriptorAllocatorDesc
    {
        struct IRenderDevice* device = nullptr;
        uint32_t maxUniformBuffers = 512;
        uint32_t maxStorageBuffers = 512;
        uint32_t maxSampledImages = 1024;
        uint32_t maxStorageImages = 256;
        uint32_t maxSamplers = 128;
    };

    struct IDescriptorAllocator
    {
        virtual ~IDescriptorAllocator() = default;
        virtual bool initialize(const FDescriptorAllocatorDesc& desc) = 0;
        virtual void destroy() = 0;

        virtual FDescriptorHandle allocate(EDescriptorType type) = 0;
        virtual void free(FDescriptorHandle handle) = 0;

        virtual uint64_t getAddress(FDescriptorHandle handle) const = 0;

        virtual const IBuffer* getResourceHeap() const = 0;
        virtual const IBuffer* getSamplerHeap() const = 0;
    protected:
        struct FRegion
        {
            uint64_t baseAddress = 0;
            uint64_t capacity = 0;
            uint64_t descriptorSize = 0;
        };
    };
}