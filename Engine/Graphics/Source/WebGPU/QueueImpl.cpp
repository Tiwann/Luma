#include "Luma/WebGPU/QueueImpl.h"
#include "Luma/WebGPU/GpuDeviceImpl.h"
#include <webgpu/webgpu.h>


namespace Luma::WebGPU
{
    FQueueImpl::FQueueImpl(FGpuDeviceImpl* device)
        : m_Device(device)
    {

    }

    bool FQueueImpl::initialize()
    {
        if (!m_Device) return false;
        m_Handle = wgpuDeviceGetQueue(m_Device->getHandle());
        if (!m_Handle) return false;
        return true;
    }

    void FQueueImpl::waitIdle()
    {
        // noop
    }

    bool FQueueImpl::executeCommandBuffers(const FQueueExecuteInfo& executeInfo)
    {

        //wgpuQueueSubmit(m_Handle, executeInfo.cmdBuffers.count(), )
        WGPUQueueWorkDoneCallbackInfo callbackInfo {};
        callbackInfo.userdata1 = this;
        callbackInfo.mode = WGPUCallbackMode_WaitAnyOnly;
        callbackInfo.callback = [](WGPUQueueWorkDoneStatus status, WGPUStringView message, void* userdata1, void* userdata2)
        {
            FQueueImpl* thisQueue = static_cast<FQueueImpl*>(userdata1);

        };


        wgpuQueueOnSubmittedWorkDone(m_Handle, callbackInfo);

        return true;
    }

    void FQueueImpl::setName(FStringView name)
    {
        wgpuQueueSetLabel(m_Handle, {*name, name.count()});
    }
}
