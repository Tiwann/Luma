#include "BufferImpl.h"
#include "RenderDeviceImpl.h"
#include <vma/vk_mem_alloc.h>


namespace Luma::Vulkan
{
    static VkBufferUsageFlags getBufferUsage(const EBufferUsage& usage)
    {
        switch (usage)
        {
        case EBufferUsage::None: return 0;
        case EBufferUsage::VertexBuffer: return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        case EBufferUsage::IndexBuffer: return VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        case EBufferUsage::UniformBuffer: return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        case EBufferUsage::StorageBuffer: return VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        case EBufferUsage::StagingBuffer: return VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        case EBufferUsage::IndirectBuffer: return VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        default: return 0;
        }
    }

    bool FBufferImpl::initialize(const FBufferDesc& bufferDesc)
    {
        VkBufferCreateInfo bufferCreateInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
        bufferCreateInfo.size = bufferDesc.size;
        bufferCreateInfo.usage = getBufferUsage(bufferDesc.usage);

        VmaAllocationCreateInfo bufferAllocationCreateInfo = {};

        switch (bufferDesc.usage)
        {
        case EBufferUsage::None:
            return false;
        case EBufferUsage::VertexBuffer:
        case EBufferUsage::IndexBuffer:
        case EBufferUsage::UniformBuffer:
        case EBufferUsage::StorageBuffer:
        case EBufferUsage::IndirectBuffer:
            {
                bufferAllocationCreateInfo.priority = 1.0f;
                bufferAllocationCreateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
                if (bufferDesc.alwaysMapped)
                {
                    bufferAllocationCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
                    bufferAllocationCreateInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
                }
            }
            break;
        case EBufferUsage::StagingBuffer:
            {
                bufferAllocationCreateInfo.priority = 0.5f;
                bufferAllocationCreateInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
                bufferAllocationCreateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;
                if (bufferDesc.alwaysMapped)
                {
                    bufferAllocationCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
                    bufferAllocationCreateInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
                }
            }
            break;
        }

        FRenderDeviceImpl* device = static_cast<FRenderDeviceImpl*>(bufferDesc.device);
        const VmaAllocator allocatorHandle = device->getAllocator();
        
        vmaDestroyBuffer(allocatorHandle, m_Handle, m_Allocation);
        VmaAllocationInfo allocationInfo;
        if (VK_FAILED(vmaCreateBuffer(allocatorHandle, &bufferCreateInfo, &bufferAllocationCreateInfo, &m_Handle, &m_Allocation, &allocationInfo)))
            return false;

        m_Device = device;
        m_Size = bufferDesc.size;
        m_Usage = bufferDesc.usage;
        m_AlwaysMapped = bufferDesc.alwaysMapped;
        m_MappedData = m_AlwaysMapped ? allocationInfo.pMappedData : nullptr;
        return true;
    }

    void FBufferImpl::destroy()
    {
        const VmaAllocator allocatorHandle = m_Device->getAllocator();
        vmaDestroyBuffer(allocatorHandle, m_Handle, m_Allocation);
        m_Handle = nullptr;
        m_Allocation = nullptr;
    }

    void* FBufferImpl::map()
    {
        if (m_AlwaysMapped) return m_MappedData;
        const VmaAllocator allocatorHandle = m_Device->getAllocator();
        void* mappedMemory = nullptr;
        vmaMapMemory(allocatorHandle, m_Allocation, &mappedMemory);
        return mappedMemory;
    }

    void FBufferImpl::unmap(const void* ptr)
    {
        if (m_AlwaysMapped) return;
        const VmaAllocator allocatorHandle = m_Device->getAllocator();
        vmaUnmapMemory(allocatorHandle, m_Allocation);
    }

    uint64_t FBufferImpl::getDeviceAddress() const
    {
        if (!m_Device) return 0;
        if (!m_Handle) return 0;
        const VkDevice deviceHandle = m_Device->getHandle();

        VkBufferDeviceAddressInfo addressInfo = { VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO };
        addressInfo.buffer = m_Handle;
        return vkGetBufferDeviceAddress(deviceHandle, &addressInfo);
    }

    VkBuffer FBufferImpl::getHandle() const
    {
        return m_Handle;
    }

    const VkBuffer* FBufferImpl::getHandlePtr() const
    {
        return &m_Handle;
    }

    void FBufferImpl::getAllocationInfo(VmaAllocationInfo2& outAllocationInfo) const
    {
        const VmaAllocator allocatorHandle = m_Device->getAllocator();
        vmaGetAllocationInfo2(allocatorHandle, m_Allocation, &outAllocationInfo);
    }
}
