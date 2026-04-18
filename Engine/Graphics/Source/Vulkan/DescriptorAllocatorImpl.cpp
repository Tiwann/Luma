#include "DescriptorAllocatorImpl.h"
#include "RenderDeviceImpl.h"
#include "Luma/Runtime/Assertion.h"
#include <Volk/volk.h>


namespace Luma::Vulkan
{
    template<IntegerType T>
    T alignUp(T value, T alignment)
    {
        return (value + alignment - 1) & ~(alignment - 1);
    }

    bool FDescriptorAllocatorImpl::initialize(const FDescriptorAllocatorDesc& allocatorDesc)
    {
        if (!allocatorDesc.device) return false;
        FRenderDeviceImpl* device = static_cast<FRenderDeviceImpl*>(allocatorDesc.device);
        const VkPhysicalDevice physicalDevice = device->getPhysicalDevice();
        if (!physicalDevice) return false;

        VkPhysicalDeviceDescriptorHeapPropertiesEXT heapProps = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_HEAP_PROPERTIES_EXT };
        VkPhysicalDeviceProperties2 properties = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2 };
        properties.pNext = &heapProps;
        vkGetPhysicalDeviceProperties2(physicalDevice, &properties);

        m_BufferRegion.baseAddress = 0;
        m_BufferRegion.capacity = allocatorDesc.maxUniformBuffers + allocatorDesc.maxStorageBuffers;
        m_BufferRegion.descriptorSize = heapProps.bufferDescriptorSize;

        /*m_SamplerRegion.baseAddress = m_BufferRegion.baseAddress + m_BufferRegion.capacity * m_BufferRegion.descriptorSize;
        m_SamplerRegion.capacity = allocatorDesc.max;
        m_SamplerRegion.descriptorSize = 0;

        m_ImageRegion.baseAddress = 0;
        m_ImageRegion.capacity = 1;
        m_ImageRegion.descriptorSize = 0;*/

        const uint64_t resourceAlignment = std::max(heapProps.bufferDescriptorSize, heapProps.imageDescriptorSize);

        const auto getResourceHeapSize = [&heapProps, &allocatorDesc, &resourceAlignment] -> uint64_t
        {
            uint64_t heapSize = 0;
            heapSize += alignUp(allocatorDesc.maxUniformBuffers * heapProps.bufferDescriptorSize, resourceAlignment);
            heapSize += alignUp(allocatorDesc.maxStorageBuffers * heapProps.bufferDescriptorSize, resourceAlignment);
            heapSize += alignUp(allocatorDesc.maxStorageImages * heapProps.imageDescriptorSize, resourceAlignment);
            heapSize += alignUp(allocatorDesc.maxSampledImages * heapProps.imageDescriptorSize, resourceAlignment);
            heapSize += heapProps.minResourceHeapReservedRange;
            return heapSize;
        };

        uint64_t resourceHeapSize = getResourceHeapSize();
        assert(resourceHeapSize <= heapProps.maxResourceHeapSize, "Resource heap size too big!");

        FBufferDesc resourceHeapDesc;
        resourceHeapDesc.device = device;
        resourceHeapDesc.usage = EBufferUsage::DescriptorHeap;
        resourceHeapDesc.alwaysMapped = true;
        resourceHeapDesc.size = resourceHeapSize;
        if (!m_ResourceHeap.initialize(resourceHeapDesc))
            return false;


        uint64_t samplerHeapSize = allocatorDesc.maxSamplers * heapProps.samplerDescriptorSize + heapProps.minSamplerHeapReservedRange;
        assert(samplerHeapSize <= heapProps.maxSamplerHeapSize, "Sampler heap size too big!");
        FBufferDesc samplerHeapDesc;
        samplerHeapDesc.device = device;
        samplerHeapDesc.usage = EBufferUsage::DescriptorHeap;
        samplerHeapDesc.alwaysMapped = true;
        samplerHeapDesc.size = samplerHeapSize;
        if (!m_SamplerHeap.initialize(samplerHeapDesc))
            return false;

        return true;
    }

    void FDescriptorAllocatorImpl::destroy()
    {
    }

    FDescriptorHandle FDescriptorAllocatorImpl::allocate(EDescriptorType type)
    {
    }

    void FDescriptorAllocatorImpl::free(FDescriptorHandle handle)
    {
        m_ResourceHeap.destroy();
        m_SamplerHeap.destroy();
    }

    uint64_t FDescriptorAllocatorImpl::getAddress(FDescriptorHandle handle) const
    {
    }

    const IBuffer* FDescriptorAllocatorImpl::getResourceHeap() const
    {
        return &m_ResourceHeap;
    }

    const IBuffer* FDescriptorAllocatorImpl::getSamplerHeap() const
    {
        return &m_SamplerHeap;
    }
}
