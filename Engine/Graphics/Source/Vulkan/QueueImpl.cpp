#include "Luma/Vulkan/QueueImpl.h"
#include "Luma/Vulkan/RenderDeviceImpl.h"
#include "Luma/Vulkan/CommandBufferImpl.h"
#include "Luma/Vulkan/VulkanUtils.h"
#include <volk.h>


namespace Luma::Vulkan
{
    FQueueImpl::FQueueImpl(FRenderDeviceImpl* device) : m_Device(device)
    {

    }

    void FQueueImpl::waitIdle()
    {
        vkQueueWaitIdle(m_Handle);
    }

    bool FQueueImpl::executeCommandBuffers(const FQueueExecuteInfo& executeInfo)
    {
        TArray<VkCommandBufferSubmitInfo> cmdBufferInfos;
        for (const ICommandBuffer* cmdBuffer : executeInfo.cmdBuffers)
        {
            VkCommandBufferSubmitInfo submitInfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO};
            submitInfo.commandBuffer = static_cast<const FCommandBufferImpl*>(cmdBuffer)->getHandle();
            cmdBufferInfos.add(submitInfo);
        }

        TArray<VkSemaphoreSubmitInfo> waitInfos;
        for (const FFenceWait& wait : executeInfo.waits)
        {
            VkSemaphoreSubmitInfo semaphoreInfo{VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO};
            semaphoreInfo.semaphore = static_cast<const FFenceImpl*>(wait.fence)->getHandle();
            semaphoreInfo.value = wait.value;
            waitInfos.add(semaphoreInfo);
        }

        TArray<VkSemaphoreSubmitInfo> signalInfos;
        for (const FFenceWait& signal : executeInfo.signals)
        {
            VkSemaphoreSubmitInfo semaphoreInfo{VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO};
            semaphoreInfo.semaphore = static_cast<const FFenceImpl*>(signal.fence)->getHandle();
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

        if (VK_FAILED(vkQueueSubmit2(m_Handle, 1, &submitInfo, nullptr)))
            return false;
        return true;
    }


    VkQueue FQueueImpl::getHandle() const
    {
        return m_Handle;
    }

    VkQueue* FQueueImpl::getHandlePtr()
    {
        return &m_Handle;
    }

    const VkQueue* FQueueImpl::getHandlePtr() const
    {
        return &m_Handle;
    }

    void FQueueImpl::setIndex(const uint32_t index)
    {
        m_Index = index;
    }

    uint32_t FQueueImpl::getIndex() const
    {
        return m_Index;
    }

    const uint32_t* FQueueImpl::getIndexPtr() const
    {
        return &m_Index;
    }

    bool FQueueImpl::sameIndex(const FQueueImpl& other) const
    {
        return m_Index == other.m_Index;
    }

    bool FQueueImpl::sameHandle(const FQueueImpl& other) const
    {
        return m_Handle == other.m_Handle;
    }

    bool FQueueImpl::same(const FQueueImpl& other) const
    {
        return m_Index == other.m_Index && m_Handle == other.m_Handle;
    }

    void FQueueImpl::setName(FStringView name)
    {
        setVulkanObjectDebugName(m_Device, VK_OBJECT_TYPE_QUEUE, m_Handle, name);
    }

    VkCommandPool FQueueImpl::createCommandPool()
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
