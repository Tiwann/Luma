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
    ImguiRenderer::ImguiRenderer(Ref<Window> window, Ref<RHI::Device> device)
        : Luma::ImguiRenderer(window, device)
    {
        if (Ref<DesktopWindow> desktopWindow = window.as<DesktopWindow>())
            ImGui_ImplGlfw_InitForVulkan(desktopWindow->getHandle(), true);

        Ref<Vulkan::Device> deviceImpl = device.as<Vulkan::Device>();
        const Swapchain* swapchain = static_cast<Swapchain*>(device->getSwapchain());
        Queue* renderQueue = static_cast<Queue*>(device->getRenderQueue());

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

        TextureDesc desc = TextureDesc::texture2D(window->getWidth(), window->getHeight(), swapchain->getFormat());
        desc.usageFlags = TextureUsage::RenderTarget | TextureUsage::Color | TextureUsage::Sampled;
        m_Texture = device->createTexture(desc);
        window->resizedEvent.bind([&](uint32_t w, uint32_t h)
        {
            m_Device->waitIdle();
            m_Texture->resize(w, h);
        });

        m_CmdBuff = device->createCommandBuffer(renderQueue);
        m_Fence = device->createFence(0);
        m_Device = device;
        m_Window = window;
    }

    ImguiRenderer::~ImguiRenderer()
    {
        m_Device->waitIdle();
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

    Ref<RHI::Texture> ImguiRenderer::render()
    {
        ImGui::Render();
        ImDrawData* drawData = ImGui::GetDrawData();
        if (!drawData) return nullptr;

        const auto width = m_Texture->getWidth();
        const auto height = m_Texture->getHeight();

        Ref<CommandBuffer> cmdBuffer = m_CmdBuff.as<CommandBuffer>();
        if (!cmdBuffer->begin()) return nullptr;

        cmdBuffer->beginDebugGroup("Imgui Pass", Color::Purple);

        RenderPassTarget colorTarget;
        colorTarget.loadOp = LoadOp::Clear;
        colorTarget.storeOp = StoreOp::Store;
        colorTarget.type = RenderPassTargetType::Color;
        colorTarget.clearValue.color = Color::Black;
        colorTarget.textureView = m_Texture->getTextureView();

        RenderPassDesc passDesc;
        passDesc.colorTargets.add(&colorTarget);
        passDesc.renderArea = FRect2u(0, 0, m_Texture->getWidth(), m_Texture->getHeight());
        cmdBuffer->beginRenderPass(passDesc);

        cmdBuffer->setViewport(Viewport::fromSize(width, height));
        cmdBuffer->setScissor(Scissor::fromSize(width, height));
        ImGui_ImplVulkan_RenderDrawData(drawData, cmdBuffer->getHandle());

        cmdBuffer->endRenderPass();
        cmdBuffer->endDebugGroup();
        cmdBuffer->end();

        RHI::Queue* renderQueue = m_Device->getRenderQueue();
        static uint16_t fenceValue = 0;

        FenceSync signal;
        signal.value = fenceValue++;
        signal.stages = PipelineStages::ColorTargetOutput;
        signal.fence = m_Fence;

        const Array<const RHI::CommandBuffer*> cmdBuffers{m_CmdBuff};
        const Array<FenceSync> signals{std::move(signal)};

        QueueExecuteInfo execInfo;
        execInfo.cmdBuffers = cmdBuffers;
        execInfo.signals = signals;
        renderQueue->executeCommandBuffers(execInfo);

        m_Fence->waitOnCPU(fenceValue);

        return m_Texture;
    }

    void ImguiRenderer::drawTexture(const RHI::TextureView* textureView, const FVector2f& uv0, const FVector2f& uv1)
    {
        const ImTextureID textureId = getOrAddTexture(textureView);
        ImGui::Image(textureId, ImVec2(textureView->getWidth(), textureView->getHeight()), {uv0.x, uv0.y}, {uv1.x, uv1.y});
    }

    uint64_t ImguiRenderer::addTexture(const RHI::TextureView* texture)
    {
        const TextureView* textureImpl = static_cast<const TextureView*>(texture);
        VkDescriptorSet descriptorSet = ImGui_ImplVulkan_AddTexture(m_Sampler->getHandle(), textureImpl->getHandle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
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
