#include "Luma/Deko3D/QueueImpl.h"
#include "Luma/Deko3D/GpuDeviceImpl.h"
#include "Luma/Deko3D/CommandBufferImpl.h"
#include "Luma/Deko3D/SemaphoreImpl.h"
#include "Luma/Deko3D/Conversions.h"
#include <deko3d.h>

namespace Luma::Deko3d
{
    bool FQueueImpl::initialize(FGpuDeviceImpl* device, EQueueType queueType)
    {
        const DkDevice deviceHandle = device->getHandle();
        if (!deviceHandle) return false;

        DkQueueMaker maker;
        dkQueueMakerDefaults(&maker, deviceHandle);
        maker.flags = convert<uint32_t>(queueType);
        m_Handle = dkQueueCreate(&maker);
        if (!m_Handle) return false;

        m_Device = device;
        m_QueueType = queueType;
        return true;
    }

    void FQueueImpl::destroy()
    {
        dkQueueDestroy(m_Handle);
        m_Handle = nullptr;
        m_Device = nullptr;
        m_QueueType = EQueueType::None;
    }

    bool FQueueImpl::executeCommandBuffer(const ICommandBuffer* cmdBuffer, IFence* signalFence, FPipelineStageFlags stageMask)
    {
        if (!cmdBuffer) return false;

        const FCommandBufferImpl* cmdBuf = static_cast<const FCommandBufferImpl*>(cmdBuffer);
        const DkCmdList listHandle = cmdBuf->getListHandle();
        if (!listHandle) return false;
        dkQueueSubmitCommands(m_Handle, listHandle);
        return true;
    }

    bool FQueueImpl::present(ISwapchain* swapchain, ISemaphore* waitSemaphore, uint32_t imageIndex)
    {
        (void)waitSemaphore;
        const DkSwapchain swapchainHandle = static_cast<FSwapchainImpl*>(swapchain)->getHandle();
        dkQueuePresentImage(m_Handle, swapchainHandle, imageIndex);
        return true;
    }

    void FQueueImpl::waitForSemaphore(const ISemaphore* semaphore)
    {
        if (!semaphore) return;
        const DkFence fence = static_cast<const FSemaphoreImpl*>(semaphore)->getHandle();
        if (fence) dkQueueWaitFence(m_Handle, fence);
    }

    void FQueueImpl::signalSemaphore(const ISemaphore* semaphore)
    {
        if (!semaphore) return;
        const DkFence fence = static_cast<const FSemaphoreImpl*>(semaphore)->getHandle();
        if (fence) dkQueueSignalFence(m_Handle, fence, true);
    }

    DkQueue FQueueImpl::getHandle() const
    {
        return m_Handle;
    }

    const DkQueue* FQueueImpl::getHandlePtr() const
    {
        return &m_Handle;
    }

    void FQueueImpl::setName(FStringView name)
    {
        (void)name;
    }

    void FQueueImpl::waitIdle()
    {
        dkQueueWaitIdle(m_Handle);
    }
}
