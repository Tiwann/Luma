#include "QueueImpl.h"
#include "CommandBufferImpl.h"
#include "SwapchainImpl.h"
#include "FenceImpl.h"
#include <Volk/volk.h>

#include "RenderDeviceImpl.h"
#include "Luma/Runtime/Assertion.h"


namespace Luma::Vulkan
{
    void FQueueImpl::submit(const FCommandBufferImpl* cmdBuffer, VkSemaphore waitSemaphore, VkSemaphore signalSemaphore, FFenceImpl* fence, const uint32_t waitStagesMask) const
    {
        if (!cmdBuffer) return;

        const VkCommandBuffer commandBuffers[] { cmdBuffer->getHandle() };
        const VkSemaphore waitSemaphores[] { waitSemaphore };
        const VkSemaphore signalSemaphores[] { signalSemaphore };
        const uint32_t waitStagesMasks[] = { waitStagesMask };

        VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
        submitInfo.commandBufferCount = std::size(commandBuffers);
        submitInfo.pCommandBuffers = commandBuffers;
        submitInfo.waitSemaphoreCount = waitSemaphore ? 1 : 0;
        submitInfo.pWaitSemaphores = waitSemaphore ? waitSemaphores : nullptr;
        submitInfo.signalSemaphoreCount = signalSemaphore ? 1 : 0;
        submitInfo.pSignalSemaphores = signalSemaphore ? signalSemaphores : nullptr;
        submitInfo.pWaitDstStageMask = waitStagesMasks;
        vkQueueSubmit(m_Handle, 1, &submitInfo, fence ? fence->getHandle() : nullptr);
    }

    bool FQueueImpl::present(const FSwapchainImpl* swapchain, const VkSemaphore waitSemaphore, const uint32_t imageIndex) const
    {
        const uint32_t imageIndices[] = { imageIndex };
        VkPresentInfoKHR presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapchain->getHandlePtr();
        presentInfo.pImageIndices = imageIndices;

        if (waitSemaphore)
        {
            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = &waitSemaphore;
        }

        if (VK_FAILED(vkQueuePresentKHR(m_Handle, &presentInfo)))
        {
            assert(false, "Something went wrong!")
            return false;
        }
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

    bool FQueueImpl::operator==(const FQueueImpl& other) const
    {
        return m_Index == other.m_Index && m_Handle == other.m_Handle;
    }
}
