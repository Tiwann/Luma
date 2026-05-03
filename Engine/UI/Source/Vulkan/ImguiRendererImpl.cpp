#include "Luma/Vulkan/ImguiRendererImpl.h"
#include "Luma/Runtime/DesktopWindow.h"
#include "Luma/Rendering/ImguiRenderer.h"
#include "Luma/Rendering/Swapchain.h"
#include "Luma/Vulkan/SamplerImpl.h"
#include "Luma/Vulkan/RenderDeviceImpl.h"
#include "Luma/Vulkan/Conversions.h"

#include <rgfw/rgfw.h>
#include <imgui_impl_vulkan.h>
#define RGFW_IMGUI_IMPLEMENTATION
#include <rgfw/imgui_impl_rgfw.h>

namespace Luma::Vulkan
{
    bool FImguiRendererImpl::initialize(const FImguiRendererDesc& rendererDesc)
    {
        if (!IImguiRenderer::initialize(rendererDesc)) return false;

        if (FDesktopWindow* desktopWindow = dynamic_cast<FDesktopWindow*>(rendererDesc.window))
        {
            if(!ImGui_ImplRgfw_InitForVulkan(desktopWindow->getHandle(), true))
                return false;
        }

        FRenderDeviceImpl* device = static_cast<FRenderDeviceImpl*>(rendererDesc.device);
        const FSwapchainImpl* swapchain = static_cast<FSwapchainImpl*>(device->getSwapchain());
        const FQueueImpl* renderQueue = static_cast<FQueueImpl*>(device->getRenderQueue());

        ImGui_ImplVulkan_InitInfo initInfo{};
        initInfo.Instance = device->getInstance();
        initInfo.Device = device->getHandle();
        initInfo.PhysicalDevice = device->getPhysicalDevice();
        initInfo.Queue = renderQueue->getHandle();
        initInfo.Allocator = nullptr;
        initInfo.ImageCount = swapchain->getImageCount();
        initInfo.QueueFamily = renderQueue->getIndex();
        initInfo.MinImageCount = swapchain->getImageCount();
        initInfo.DescriptorPool = nullptr;
        initInfo.DescriptorPoolSize = 32;
        initInfo.UseDynamicRendering = true;

        initInfo.PipelineInfoMain.MSAASamples = static_cast<VkSampleCountFlagBits>(rendererDesc.sampleCount);
        initInfo.PipelineInfoMain.PipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
        initInfo.PipelineInfoMain.PipelineRenderingCreateInfo.viewMask = 0;
        initInfo.PipelineInfoMain.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
        initInfo.PipelineInfoMain.PipelineRenderingCreateInfo.depthAttachmentFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;
        initInfo.PipelineInfoMain.PipelineRenderingCreateInfo.stencilAttachmentFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;

        const VkFormat colorFormat = convert<VkFormat>(swapchain->getFormat());
        initInfo.PipelineInfoMain.PipelineRenderingCreateInfo.pColorAttachmentFormats = &colorFormat;

        initInfo.CheckVkResultFn = [](const VkResult result)
        {
            if(result != VK_SUCCESS)
            {
                std::println(std::cerr, "Something went wrong.");
            }
        };

        if(!ImGui_ImplVulkan_Init(&initInfo))
            return false;


        const FSamplerDesc samplerDesc = FSamplerDesc()
        .withAddressMode(ESamplerAddressMode::Repeat)
        .withFilter(EFilter::Linear, EFilter::Linear);

        m_Sampler = device->getOrCreateSampler(samplerDesc);
        if (!m_Sampler) return false;

        m_Device = device;
        return true;
    }

    void FImguiRendererImpl::destroy()
    {
        for (const auto& [_, id] : m_Textures)
            ImGui_ImplVulkan_RemoveTexture(reinterpret_cast<VkDescriptorSet>(id));

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplRgfw_Shutdown();
        ImGui::DestroyContext(m_Context);
        if (m_Sampler) m_Sampler->destroy();
    }

    void FImguiRendererImpl::beginFrame()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplRgfw_NewFrame();
        ImGui::NewFrame();
    }

    void FImguiRendererImpl::endFrame()
    {
        ImGui::EndFrame();
    }

    void FImguiRendererImpl::render(ICommandBuffer* cmdBuffer)
    {
        ImGui::Render();
        ImDrawData* drawData = ImGui::GetDrawData();
        if (!drawData) return;

        const FCommandBufferImpl* cmdBufferImpl = static_cast<FCommandBufferImpl*>(cmdBuffer);
        const uint32_t width = m_Device->getSwapchain()->getWidth();
        const uint32_t height = m_Device->getSwapchain()->getHeight();
        cmdBuffer->setViewport(FViewport(0, 0, width, height, 0.0f, 1.0f));
        cmdBuffer->setScissor(FScissor(0, 0, width, height));
        ImGui_ImplVulkan_RenderDrawData(drawData, cmdBufferImpl->getHandle());
    }

    void FImguiRendererImpl::drawTexture(ITextureView* textureView, const FVector2f& uv0, const FVector2f& uv1)
    {
        const ImTextureID textureId = getOrAddTexture(textureView);
        ImGui::Image(textureId, ImVec2(textureView->getWidth(), textureView->getHeight()), {uv0.x, uv0.y}, {uv1.x, uv1.y});
    }

    uint64_t FImguiRendererImpl::addTexture(const ITextureView* texture)
    {
        const FTextureViewImpl* textureImpl = static_cast<const FTextureViewImpl*>(texture);
        FSamplerImpl* samplerImpl = static_cast<FSamplerImpl*>(m_Sampler);
        VkDescriptorSet descriptorSet = ImGui_ImplVulkan_AddTexture(samplerImpl->getHandle(), textureImpl->getHandle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        if (!descriptorSet) return 0;

        const ImTextureID textureId = reinterpret_cast<ImTextureID>(descriptorSet);
        m_Textures[texture] = textureId;
        return textureId;
    }

    uint64_t FImguiRendererImpl::getOrAddTexture(const ITextureView* texture)
    {
        return m_Textures.contains(texture) ? m_Textures[texture] : addTexture(texture);
    }
}
