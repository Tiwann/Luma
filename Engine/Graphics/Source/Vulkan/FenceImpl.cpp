#include "FenceImpl.h"
#include "RenderDeviceImpl.h"
#include "VulkanUtils.h"
#include <Volk/volk.h>


namespace Luma::Vulkan
{
    bool FFenceImpl::initialize(const FFenceDesc& fenceDesc)
    {
        FRenderDeviceImpl* device = static_cast<FRenderDeviceImpl*>(fenceDesc.device);
        const VkDevice deviceHandle = device->getHandle();

        VkFenceCreateInfo fenceCreateInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
        fenceCreateInfo.flags = fenceDesc.signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
        vkDestroyFence(deviceHandle, m_Handle, nullptr);

        const VkResult result = vkCreateFence(deviceHandle, &fenceCreateInfo, nullptr, &m_Handle);
        if (result != VK_SUCCESS)
            return false;

        m_Device = device;
        return true;
    }

    void FFenceImpl::destroy()
    {
        const VkDevice deviceHandle = m_Device->getHandle();
        vkDestroyFence(deviceHandle, m_Handle, nullptr);
        m_Device = nullptr;
        m_Handle = nullptr;
    }

    void FFenceImpl::wait(const uint64_t timeoutNs)
    {
        const VkDevice deviceHandle = m_Device->getHandle();
        vkWaitForFences(deviceHandle, 1, &m_Handle, true, timeoutNs);
    }

    void FFenceImpl::reset()
    {
        const VkDevice deviceHandle = m_Device->getHandle();
        vkResetFences(deviceHandle, 1, &m_Handle);
    }

    void FFenceImpl::setName(FStringView name)
    {
        setVulkanObjectDebugName(m_Device, VK_OBJECT_TYPE_COMMAND_BUFFER, m_Handle, name);
    }
}
