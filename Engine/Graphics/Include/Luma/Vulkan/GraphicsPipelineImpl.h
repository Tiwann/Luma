#pragma once
#include "Luma/Rendering/GraphicsPipeline.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class FRenderDeviceImpl;

    class FGraphicsPipelineImpl : public IGraphicsPipeline
    {
    public:
        FGraphicsPipelineImpl() = default;
        ~FGraphicsPipelineImpl() override = default;

        bool initialize(const FGraphicsPipelineDesc& pipelineDesc) override;
        void destroy() override;

        VkPipeline getHandle() const { return m_Handle; }
    private:
        FRenderDeviceImpl* m_Device = nullptr;
        VkPipeline m_Handle = nullptr;
    };
}
