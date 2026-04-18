#pragma once
#include "Luma/Rendering/DescriptorAllocator.h"
#include "BufferImpl.h"

namespace Luma::Vulkan
{
    class FDescriptorAllocatorImpl : public IDescriptorAllocator
    {
    public:
        static constexpr uint32_t DESCRIPTOR_TYPE_COUNT = 5;

        bool initialize(const FDescriptorAllocatorDesc& allocatorDesc) override;
        void destroy() override;
        FDescriptorHandle allocate(EDescriptorType type) override;
        void free(FDescriptorHandle handle) override;
        uint64_t getAddress(FDescriptorHandle handle) const override;
        const IBuffer* getResourceHeap() const override;
        const IBuffer* getSamplerHeap() const override;

    private:
        FBufferImpl m_ResourceHeap;
        FBufferImpl m_SamplerHeap;

        FRegion m_BufferRegion{};
        FRegion m_ImageRegion{};
        FRegion m_SamplerRegion{};
    };
}
