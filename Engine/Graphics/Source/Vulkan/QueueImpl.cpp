#include "QueueImpl.h"
#include "RenderDeviceImpl.h"
#include "SemaphoreImpl.h"
#include "CommandBufferImpl.h"
#include "VulkanUtils.h"
#include <Volk/volk.h>


namespace Luma::Vulkan
{
    bool FQueueImpl::executeCommandBuffer(const ICommandBuffer* cmdBuffer, IFence* signalFence, FPipelineStageFlags stageMask)
    {
        if (!cmdBuffer) return false;

        const VkCommandBuffer cmdBuffers[] { static_cast<const FCommandBufferImpl*>(cmdBuffer)->getHandle() };
        const auto toSemaphoreHandle = [](const FSemaphoreImpl* semaphore) -> VkSemaphore { return semaphore->getHandle(); };
        TArray<VkSemaphore> waitSemaphores = m_WaitSemaphores.transform<VkSemaphore>(toSemaphoreHandle);
        TArray<VkSemaphore> signalSemaphores = m_SignalSemaphores.transform<VkSemaphore>(toSemaphoreHandle);

        const VkPipelineStageFlags vkStageMask = stageMask.as<VkPipelineStageFlags>();
        VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = cmdBuffers;
        submitInfo.pWaitSemaphores = waitSemaphores.data();
        submitInfo.waitSemaphoreCount = waitSemaphores.count();
        submitInfo.pSignalSemaphores = signalSemaphores.data();
        submitInfo.signalSemaphoreCount = signalSemaphores.count();
        submitInfo.pWaitDstStageMask = &vkStageMask;

        if (VK_FAILED(vkQueueSubmit(m_Handle, 1, &submitInfo, signalFence ? static_cast<FFenceImpl*>(signalFence)->getHandle() : nullptr)))
            return false;

        m_WaitSemaphores.clear();
        m_SignalSemaphores.clear();
        return true;
    }

    bool FQueueImpl::present(ISwapchain* swapchain, ISemaphore* waitSemaphore, uint32_t imageIndex)
    {
        if (!swapchain) return false;

        const VkSemaphore waitSemaphores[]{ static_cast<const FSemaphoreImpl*>(waitSemaphore)->getHandle() };

        VkResult result = VK_SUCCESS;
        VkPresentInfoKHR presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
        presentInfo.pSwapchains = static_cast<FSwapchainImpl*>(swapchain)->getHandlePtr();
        presentInfo.swapchainCount = 1;
        presentInfo.pWaitSemaphores = waitSemaphores;
        presentInfo.waitSemaphoreCount = std::size(waitSemaphores);
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = &result;

        if (VK_FAILED(vkQueuePresentKHR(m_Handle, &presentInfo)))
            return false;

        if (VK_FAILED(result))
            return false;

        return true;
    }

    void FQueueImpl::waitForSemaphore(const ISemaphore* semaphore)
    {
        m_WaitSemaphores.addUnique(static_cast<const FSemaphoreImpl*>(semaphore));
    }

    void FQueueImpl::signalSemaphore(const ISemaphore* semaphore)
    {
        m_SignalSemaphores.addUnique(static_cast<const FSemaphoreImpl*>(semaphore));
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
}
