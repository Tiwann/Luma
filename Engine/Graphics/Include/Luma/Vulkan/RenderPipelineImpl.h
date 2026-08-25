#pragma once
#include "Luma/Rendering/RenderPipeline.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class FRenderDeviceImpl;

    class FRenderPipelineImpl : public IRenderPipeline
    {
    public:
        FRenderPipelineImpl() = default;
        ~FRenderPipelineImpl() override = default;

        bool initialize(const FRenderPipelineDesc& pipelineDesc) override;
        void destroy() override;

        VkPipeline getHandle() const { return m_Handle; }
    private:
        FRenderDeviceImpl* m_Device = nullptr;
        VkPipeline m_Handle = nullptr;
    };
}
