#include "Luma/Vulkan/ImguiRenderer.h"
#include "Luma/Runtime/DesktopWindow.h"
#include "Luma/Rendering/ImguiRenderer.h"
#include "Luma/Rendering/Swapchain.h"
#include "Luma/Vulkan/Sampler.h"
#include "Luma/Vulkan/Device.h"
#include "Luma/Vulkan/Conversions.h"

#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>

using namespace Luma::RHI;

namespace Luma::Vulkan
{
    ImguiRenderer::ImguiRenderer(Window* window, RHI::Device* device)
    {
        if (DesktopWindow* desktopWindow = dynamic_cast<DesktopWindow*>(window))
            ImGui_ImplGlfw_InitForVulkan(desktopWindow->getHandle(), true);

        Device* deviceImpl = static_cast<Device*>(device);
        const Swapchain* swapchain = static_cast<Swapchain*>(device->getSwapchain());
        const Queue* renderQueue = static_cast<Queue*>(device->getRenderQueue());

        ImGui_ImplVulkan_InitInfo initInfo{};
        initInfo.Instance = deviceImpl->getInstance();
        initInfo.Device = deviceImpl->getHandle();
        initInfo.PhysicalDevice = deviceImpl->getPhysicalDevice();
        initInfo.Queue = renderQueue->getHandle();
        initInfo.Allocator = nullptr;
        initInfo.ImageCount = swapchain->getTextureCount();
        initInfo.QueueFamily = renderQueue->getIndex();
        initInfo.MinImageCount = swapchain->getTextureCount();
        initInfo.DescriptorPool = nullptr;
        initInfo.DescriptorPoolSize = 32;
        initInfo.UseDynamicRendering = true;

        initInfo.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
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

        ImGui_ImplVulkan_Init(&initInfo);


        const SamplerDesc samplerDesc = SamplerDesc()
        .withAddressMode(SamplerAddressMode::Repeat)
        .withFilter(Filter::Linear, Filter::Linear);

        m_Sampler = device->getOrCreateSampler(samplerDesc);
        m_Device = device;
    }

    ImguiRenderer::~ImguiRenderer()
    {
        for (const auto& [_, id] : m_Textures)
            ImGui_ImplVulkan_RemoveTexture(reinterpret_cast<VkDescriptorSet>(id));

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext(m_Context);
    }

    void ImguiRenderer::beginFrame()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImguiRenderer::endFrame()
    {
        ImGui::EndFrame();
        ImGui::UpdatePlatformWindows();
    }

    void ImguiRenderer::render(RHI::CommandBuffer* cmdBuffer)
    {
        ImGui::Render();
        ImDrawData* drawData = ImGui::GetDrawData();
        if (!drawData) return;

        const CommandBuffer* cmdBufferImpl = static_cast<CommandBuffer*>(cmdBuffer);
        const RHI::Swapchain* swapchain = m_Device->getSwapchain();
        cmdBuffer->setViewport(Viewport(swapchain->getBounds().as<float>()));
        cmdBuffer->setScissor(Scissor(swapchain->getBounds()));
        ImGui_ImplVulkan_RenderDrawData(drawData, cmdBufferImpl->getHandle());
    }

    void ImguiRenderer::drawTexture(const RHI::TextureView* textureView, const FVector2f& uv0, const FVector2f& uv1)
    {
        const ImTextureID textureId = getOrAddTexture(textureView);
        ImGui::Image(textureId, ImVec2(textureView->getWidth(), textureView->getHeight()), {uv0.x, uv0.y}, {uv1.x, uv1.y});
    }

    uint64_t ImguiRenderer::addTexture(const RHI::TextureView* texture)
    {
        const TextureView* textureImpl = static_cast<const TextureView*>(texture);
        Sampler* samplerImpl = static_cast<Sampler*>(m_Sampler);
        VkDescriptorSet descriptorSet = ImGui_ImplVulkan_AddTexture(samplerImpl->getHandle(), textureImpl->getHandle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        if (!descriptorSet) return 0;

        const ImTextureID textureId = reinterpret_cast<ImTextureID>(descriptorSet);
        m_Textures[texture] = textureId;
        return textureId;
    }

    uint64_t ImguiRenderer::getOrAddTexture(const RHI::TextureView* texture)
    {
        return m_Textures.contains(texture) ? m_Textures[texture] : addTexture(texture);
    }
}
