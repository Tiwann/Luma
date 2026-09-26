#pragma once
#include "Luma/Rendering/ComputePipeline.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class Device;

    class ComputePipeline : public RHI::ComputePipeline
    {
    public:
        bool initialize(const RHI::ComputePipelineDesc& pipelineDesc) override;
        void destroy() override;

        VkPipeline getHandle() const { return m_Handle; }
    private:
        Device* m_Device = nullptr;
        VkPipeline m_Handle = nullptr;
    };
}

