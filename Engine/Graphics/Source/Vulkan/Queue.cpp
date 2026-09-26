#include "Luma/Vulkan/Queue.h"
#include "Luma/Vulkan/Device.h"
#include "Luma/Vulkan/CommandBuffer.h"
#include "Luma/Vulkan/VulkanUtils.h"
#include <volk.h>

#include "Luma/Vulkan/Fence.h"


namespace Luma::Vulkan
{
    Queue::Queue(Device* device) : m_Device(device)
    {

    }

    void Queue::waitIdle()
    {
        vkQueueWaitIdle(m_Handle);
    }

    bool Queue::executeCommandBuffers(const RHI::QueueExecuteInfo& executeInfo)
    {
        Array<VkCommandBufferSubmitInfo> cmdBufferInfos;
        for (const RHI::CommandBuffer* cmdBuffer : executeInfo.cmdBuffers)
        {
            VkCommandBufferSubmitInfo submitInfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO};
            submitInfo.commandBuffer = static_cast<const CommandBuffer*>(cmdBuffer)->getHandle();
            cmdBufferInfos.add(submitInfo);
        }

        Array<VkSemaphoreSubmitInfo> waitInfos;
        for (const RHI::FenceSync& wait : executeInfo.waits)
        {
            VkSemaphoreSubmitInfo semaphoreInfo{VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO};
            semaphoreInfo.semaphore = static_cast<const Fence*>(wait.fence)->getHandle();
            semaphoreInfo.value = wait.value;
            waitInfos.add(semaphoreInfo);
        }

        Array<VkSemaphoreSubmitInfo> signalInfos;
        for (const RHI::FenceSync& signal : executeInfo.signals)
        {
            VkSemaphoreSubmitInfo semaphoreInfo{VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO};
            semaphoreInfo.semaphore = static_cast<const Fence*>(signal.fence)->getHandle();
            semaphoreInfo.value = signal.value;
            signalInfos.add(semaphoreInfo);
        }

        VkSubmitInfo2 submitInfo = {VK_STRUCTURE_TYPE_SUBMIT_INFO_2};
        submitInfo.pCommandBufferInfos = cmdBufferInfos.data();
        submitInfo.commandBufferInfoCount = cmdBufferInfos.count();
        submitInfo.pWaitSemaphoreInfos = waitInfos.data();
        submitInfo.waitSemaphoreInfoCount = waitInfos.count();
        submitInfo.pSignalSemaphoreInfos = signalInfos.data();
        submitInfo.signalSemaphoreInfoCount = signalInfos.count();

        const VkResult result = vkQueueSubmit2(m_Handle, 1, &submitInfo, nullptr);
        if (VK_FAILED(result))
            return false;
        return true;
    }


    VkQueue Queue::getHandle() const
    {
        return m_Handle;
    }

    VkQueue* Queue::getHandlePtr()
    {
        return &m_Handle;
    }

    const VkQueue* Queue::getHandlePtr() const
    {
        return &m_Handle;
    }

    void Queue::setIndex(const uint32_t index)
    {
        m_Index = index;
    }

    uint32_t Queue::getIndex() const
    {
        return m_Index;
    }

    const uint32_t* Queue::getIndexPtr() const
    {
        return &m_Index;
    }

    bool Queue::sameIndex(const Queue& other) const
    {
        return m_Index == other.m_Index;
    }

    bool Queue::sameHandle(const Queue& other) const
    {
        return m_Handle == other.m_Handle;
    }

    bool Queue::same(const Queue& other) const
    {
        return m_Index == other.m_Index && m_Handle == other.m_Handle;
    }

    void Queue::setName(StringView name)
    {
        setVulkanObjectDebugName(m_Device, VK_OBJECT_TYPE_QUEUE, m_Handle, name);
    }

    VkCommandPool Queue::createCommandPool()
    {
        VkCommandPool commandPool = nullptr;

        VkCommandPoolCreateInfo createInfo{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
        createInfo.queueFamilyIndex = m_Index;
        createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        if (VK_FAILED(vkCreateCommandPool(m_Device->getHandle(), &createInfo, nullptr, &commandPool)))
            return nullptr;
        return commandPool;
    }
}
