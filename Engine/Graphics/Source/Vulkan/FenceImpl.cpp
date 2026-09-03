#include "Luma/Vulkan/FenceImpl.h"
#include "Luma/Vulkan/GpuDeviceImpl.h"
#include "Luma/Vulkan/VulkanUtils.h"
#include <volk.h>


namespace Luma::Vulkan
{
    bool FFenceImpl::initialize(const FFenceDesc& fenceDesc)
    {
        if (!fenceDesc.device) return false;

        VkSemaphoreTypeCreateInfo semaphoreExt = {VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO};
        semaphoreExt.initialValue = fenceDesc.initialValue;
        semaphoreExt.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;

        VkSemaphoreCreateInfo createInfo = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        createInfo.pNext = &semaphoreExt;
        createInfo.flags = 0;

        m_Device = static_cast<FGpuDeviceImpl*>(fenceDesc.device);
        vkDestroySemaphore(m_Device->getHandle(), m_Handle, nullptr);
        if (VK_FAILED(vkCreateSemaphore(m_Device->getHandle(), &createInfo, nullptr, &m_Handle)))
            return false;
        return true;
    }

    void FFenceImpl::destroy()
    {
        vkDestroySemaphore(m_Device->getHandle(), m_Handle, nullptr);
    }

    uint64_t FFenceImpl::getCompletedValue() const
    {
        uint64_t result = 0;
        vkGetSemaphoreCounterValue(m_Device->getHandle(), m_Handle, &result);
        return result;
    }

    void FFenceImpl::signalOnCPU(uint64_t value)
    {
        VkSemaphoreSignalInfo info = {VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO};
        info.semaphore = m_Handle;
        info.value = value;
        vkSignalSemaphore(m_Device->getHandle(), &info);
    }

    bool FFenceImpl::waitOnCPU(uint64_t value, uint64_t timeoutNs)
    {
        if (getCompletedValue() >= value) return true;

        VkSemaphoreWaitInfo info{VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO};
        info.semaphoreCount = 1;
        info.pSemaphores = &m_Handle;
        info.pValues = &value;
        return vkWaitSemaphores(m_Device->getHandle(), &info, timeoutNs) == VK_SUCCESS;
    }

    void FFenceImpl::setName(FStringView name)
    {
        setVulkanObjectDebugName(m_Device, VK_OBJECT_TYPE_SEMAPHORE, m_Handle, name);
    }
}
