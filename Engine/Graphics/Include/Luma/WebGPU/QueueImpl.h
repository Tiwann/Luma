#pragma once
#include "WgpuFwd.h"
#include "Luma/Rendering/Queue.h"

namespace Luma::WebGPU
{
    class FGPUDeviceImpl;

    class FQueueImpl : public IQueue
    {
    public:
        FQueueImpl(FGPUDeviceImpl* device);
        bool initialize();
        void waitIdle() override;
        bool executeCommandBuffers(const FQueueExecuteInfo& executeInfo) override;
        void setName(FStringView name) override;

        WGPUQueue getHandle() const { return m_Handle; }
    private:
        FGPUDeviceImpl* m_Device = nullptr;
        WGPUQueue m_Handle = nullptr;
    };
}
