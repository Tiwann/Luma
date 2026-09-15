#pragma once
#include "Luma/Rendering/RenderPipeline.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class FGPUDeviceImpl;

    class FRenderPipelineImpl : public IRenderPipeline
    {
    public:
        FRenderPipelineImpl() = default;
        ~FRenderPipelineImpl() override = default;

        bool initialize(const FRenderPipelineDesc& pipelineDesc) override;
        void destroy() override;

        VkPipeline getHandle() const { return m_Handle; }
    private:
        FGPUDeviceImpl* m_Device = nullptr;
        VkPipeline m_Handle = nullptr;
    };
}
