#include "SemaphoreImpl.h"
#include "RenderDeviceImpl.h"
#include "Luma/Runtime/Assertion.h"
#include <Volk/volk.h>

namespace Luma::Vulkan
{
    static VkSemaphoreType getSemaphoreType(const ESemaphoreType semaphoreType)
    {
        switch (semaphoreType)
        {
        case ESemaphoreType::Binary: return VK_SEMAPHORE_TYPE_BINARY;
        case ESemaphoreType::Timeline: return VK_SEMAPHORE_TYPE_TIMELINE;
        default: return VK_SEMAPHORE_TYPE_BINARY;
        }
    }
    bool FSemaphoreImpl::initialize(const FSemaphoreDesc& semaphoreDesc)
    {
        if (!semaphoreDesc.device) return false;
        FRenderDeviceImpl* device = static_cast<FRenderDeviceImpl*>(semaphoreDesc.device);

        VkSemaphoreTypeCreateInfo typeCreateInfo { VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO };
        typeCreateInfo.semaphoreType = getSemaphoreType(semaphoreDesc.semaphoreType);
        typeCreateInfo.initialValue = semaphoreDesc.semaphoreType == ESemaphoreType::Binary ? 0 : semaphoreDesc.initialValue;

        VkSemaphoreCreateInfo createInfo { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
        createInfo.pNext = &typeCreateInfo;

        vkDestroySemaphore(device->getHandle(), m_Handle, nullptr);
        if (VK_FAILED(vkCreateSemaphore(device->getHandle(), &createInfo, nullptr, &m_Handle)))
            return false;

        m_Device = device;
        m_SemaphoreType = semaphoreDesc.semaphoreType;
        return true;
    }

    void FSemaphoreImpl::destroy()
    {
        vkDestroySemaphore(m_Device->getHandle(), m_Handle, nullptr);
        m_Handle = nullptr;
    }

    uint64_t FSemaphoreImpl::getValue() const
    {
        if (!m_Device) return 0;
        uint64_t value = 0;
        vkGetSemaphoreCounterValue(m_Device->getHandle(), m_Handle, &value);
        return value;
    }

    void FSemaphoreImpl::signal(uint64_t value)
    {
        if (!m_Device) return;
        if (!m_Handle) return;
        if (m_SemaphoreType != ESemaphoreType::Timeline) return;

        const VkDevice deviceHandle = m_Device->getHandle();

        VkSemaphoreSignalInfo signalInfo { VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO };
        signalInfo.semaphore = m_Handle;
        signalInfo.value = value;
        if (VK_FAILED(vkSignalSemaphore(deviceHandle, &signalInfo)))
            assert(false, "[VULKAN] Failed to signal semaphore!")
    }

    void FSemaphoreImpl::wait(uint64_t value, uint64_t timeout)
    {
        if (!m_Device) return;
        if (!m_Handle) return;
        if (m_SemaphoreType != ESemaphoreType::Timeline) return;

        const VkDevice deviceHandle = m_Device->getHandle();
        VkSemaphoreWaitInfo waitInfo { VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO };
        waitInfo.pSemaphores = &m_Handle;
        waitInfo.semaphoreCount = 1;
        waitInfo.pValues = &value;

        if (VK_FAILED(vkWaitSemaphores(deviceHandle, &waitInfo, timeout)))
            assert(false, "[VULKAN] Failed to wait semaphore!")
    }

    void FSemaphoreImpl::setName(FStringView name)
    {
        VkDebugUtilsObjectNameInfoEXT nameInfo { VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT };
        nameInfo.objectType = VK_OBJECT_TYPE_SEMAPHORE;
        nameInfo.objectHandle = reinterpret_cast<uint64_t>(m_Handle);
        nameInfo.pObjectName = *name;
        vkSetDebugUtilsObjectNameEXT(m_Device->getHandle(), &nameInfo);
    }
}
