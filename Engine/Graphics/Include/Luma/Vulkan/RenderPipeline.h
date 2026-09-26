#pragma once
#include "Luma/Rendering/RenderPipeline.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class Device;

    class RenderPipeline : public RHI::RenderPipeline
    {
    public:
        bool initialize(const RHI::RenderPipelineDesc& pipelineDesc) override;
        void destroy() override;

        VkPipeline getHandle() const { return m_Handle; }
    private:
        Device* m_Device = nullptr;
        VkPipeline m_Handle = nullptr;
    };
}
