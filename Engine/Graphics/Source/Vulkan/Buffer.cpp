#include "Luma/Vulkan/Buffer.h"
#include "Luma/Vulkan/Device.h"
#include "Luma/Vulkan/VulkanUtils.h"
#include "Luma/Containers/StringFormat.h"
#include <vk_mem_alloc.h>


namespace Luma::Vulkan
{
    static VkBufferUsageFlags getBufferUsage(const BufferUsage& usage)
    {
        switch (usage)
        {
        case BufferUsage::None: return 0;
        case BufferUsage::VertexBuffer: return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        case BufferUsage::IndexBuffer: return VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        case BufferUsage::UniformBuffer: return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        case BufferUsage::StorageBuffer: return VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        case BufferUsage::StagingBuffer: return VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        case BufferUsage::IndirectBuffer: return VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        case BufferUsage::DescriptorBuffer:
            return VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT | VK_BUFFER_USAGE_SAMPLER_DESCRIPTOR_BUFFER_BIT_EXT
            | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
        default: return 0;
        }
    }

    bool Buffer::initialize(const RHI::BufferDesc& bufferDesc)
    {
        VkBufferCreateInfo bufferCreateInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
        bufferCreateInfo.size = bufferDesc.size;
        bufferCreateInfo.usage = getBufferUsage(bufferDesc.usage);

        VmaAllocationCreateInfo bufferAllocationCreateInfo = {};

        switch (bufferDesc.usage)
        {
        case BufferUsage::None:
            return false;
        case BufferUsage::VertexBuffer:
        case BufferUsage::IndexBuffer:
        case BufferUsage::UniformBuffer:
        case BufferUsage::StorageBuffer:
        case BufferUsage::IndirectBuffer:
        case BufferUsage::DescriptorBuffer:
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
        case BufferUsage::StagingBuffer:
            {
                bufferAllocationCreateInfo.priority = 0.5f;
                bufferAllocationCreateInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
                bufferAllocationCreateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;
            }
            break;
        }

        Device* device = static_cast<Device*>(bufferDesc.device);
        const VmaAllocator allocatorHandle = device->getAllocator();

        vmaDestroyBuffer(allocatorHandle, m_Handle, m_Allocation);
        VmaAllocationInfo allocationInfo;
        if (VK_FAILED(vmaCreateBuffer(allocatorHandle, &bufferCreateInfo, &bufferAllocationCreateInfo, &m_Handle, &m_Allocation, &allocationInfo)))
            return false;

        if (!bufferDesc.debugName.isEmpty())
            setName(bufferDesc.debugName);

        m_Device = device;
        m_Size = bufferDesc.size;
        m_Usage = bufferDesc.usage;
        m_AlwaysMapped = bufferDesc.alwaysMapped;
        m_MappedAddress = m_AlwaysMapped ? allocationInfo.pMappedData : nullptr;
        return true;
    }

    void Buffer::destroy()
    {
        const VmaAllocator allocatorHandle = m_Device->getAllocator();
        vmaDestroyBuffer(allocatorHandle, m_Handle, m_Allocation);
        m_Handle = nullptr;
        m_Allocation = nullptr;
    }

    void* Buffer::map()
    {
        if (m_AlwaysMapped) return m_MappedAddress;
        const VmaAllocator allocatorHandle = m_Device->getAllocator();
        void* mappedMemory = nullptr;
        vmaMapMemory(allocatorHandle, m_Allocation, &mappedMemory);
        return mappedMemory;
    }

    void Buffer::unmap(const void* ptr)
    {
        if (m_AlwaysMapped)
        {
            LUMA_ASSERT(ptr == m_MappedAddress, "Pointer is a not mapped data!");
            return;
        }
        const VmaAllocator allocatorHandle = m_Device->getAllocator();
        vmaUnmapMemory(allocatorHandle, m_Allocation);
    }

    uint64_t Buffer::getDeviceAddress() const
    {
        if (!m_Device) return 0;
        if (!m_Handle) return 0;
        const VkDevice deviceHandle = m_Device->getHandle();

        VkBufferDeviceAddressInfo addressInfo = { VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO };
        addressInfo.buffer = m_Handle;
        return vkGetBufferDeviceAddress(deviceHandle, &addressInfo);
    }

    VkBuffer Buffer::getHandle() const
    {
        return m_Handle;
    }

    const VkBuffer* Buffer::getHandlePtr() const
    {
        return &m_Handle;
    }

    void Buffer::getAllocationInfo(VmaAllocationInfo2& outAllocationInfo) const
    {
        const VmaAllocator allocatorHandle = m_Device->getAllocator();
        vmaGetAllocationInfo2(allocatorHandle, m_Allocation, &outAllocationInfo);
    }

    void Buffer::setName(StringView name)
    {
        setVulkanObjectDebugName(m_Device, VK_OBJECT_TYPE_BUFFER, m_Handle, name);
    }
}
