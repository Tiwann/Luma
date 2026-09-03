#pragma once
#include "Luma/Rendering/ComputePipeline.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class FGpuDeviceImpl;

    class FComputePipelineImpl : public IComputePipeline
    {
    public:
        bool initialize(const FComputePipelineDesc& pipelineDesc) override;
        void destroy() override;

        VkPipeline getHandle() const { return m_Handle; }
    private:
        FGpuDeviceImpl* m_Device = nullptr;
        VkPipeline m_Handle = nullptr;
    };
}

