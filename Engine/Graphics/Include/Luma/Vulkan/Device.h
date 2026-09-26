#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/Device.h"
#include "Luma/Rendering/ImmediateExecutor.h"
#include "Swapchain.h"
#include "Queue.h"
#include "CommandBuffer.h"
#include "VulkanFwd.h"

#define VK_FAILED(res) (res != VK_SUCCESS)

namespace Luma::Vulkan
{
    class LUMA_GRAPHICS_API Device final : public RHI::Device
    {
    public:
        DeviceType getDeviceType() override;

        bool initialize(const RHI::DeviceDesc& deviceDesc) override;
        void destroy() override;

        bool beginFrame() override;
        void endFrame() override;
        void present() override;
        void waitIdle() override;
        uint32_t getTextureCount() const override;
        uint32_t getFrameIndex() const override;

        RHI::Swapchain* getSwapchain() override;
        RHI::Queue* getRenderQueue() override;
        RHI::Queue* getComputeQueue() override;
        RHI::Queue* getCopyQueue() override;

        RHI::Buffer* createBuffer(const RHI::BufferDesc& bufferDesc) override;
        RHI::Texture* createTexture(const RHI::TextureDesc& textureDesc) override;
        RHI::TextureView* createTextureView(const RHI::TextureViewDesc& textureViewDesc) override;
        RHI::Shader* createShader(const RHI::ShaderDesc& shaderDesc) override;
        RHI::CommandBuffer* createCommandBuffer(const RHI::CommandBufferDesc& cmdBufferDesc) override;
        RHI::Sampler* createSampler(const RHI::SamplerDesc& samplerDesc) override;
        RHI::RenderPipeline* createRenderPipeline(const RHI::RenderPipelineDesc& pipelineDesc) override;
        RHI::ComputePipeline* createComputePipeline(const RHI::ComputePipelineDesc& pipelineDesc) override;
        RHI::Fence* createFence(const RHI::FenceDesc& fenceDesc) override;

        RHI::CommandBuffer* getCommandBuffer() override { return &m_CmdBuffers[m_FrameIndex]; }
        RHI::TextureView* getAcquiredSwapchainTextureView() override;
        RHI::Texture* getAcquiredSwapchainTexture() override;

        static VkInstance getInstance();
        VkDevice getHandle() const { return m_Handle; }
        VkSurfaceKHR getSurface() const { return m_Surface; }
        VkPhysicalDevice getPhysicalDevice() const { return m_PhysicalDevice; }
        VmaAllocator getAllocator() const { return m_Allocator; }
        VkCommandPool getRenderPool() const { return m_RenderPool; }
        VkCommandPool getComputePool() const { return m_ComputePool; }
        VkCommandPool getCopyPool() const { return m_CopyPool; }
        VkCommandPool getCommandPool(QueueType queueType) const;
        VkDescriptorPool getDescriptorPool() const { return m_DescriptorPool; }
    private:
        static inline VkInstance s_Instance = nullptr;
        static inline uint32_t s_DeviceCount = 0;
        static inline VkDebugUtilsMessengerEXT s_DebugMessenger = nullptr;

        VkPhysicalDevice m_PhysicalDevice = nullptr;
        VkDevice m_Handle = nullptr;
        VmaAllocator m_Allocator = nullptr;
        VkSurfaceKHR m_Surface = nullptr;
        VkFence m_Fences[RHI::NUM_FRAMES_IN_FLIGHT] = {nullptr};
        VkSemaphore m_TextureAvailableSemaphores[RHI::NUM_FRAMES_IN_FLIGHT] = {nullptr};
        VkSemaphore m_SubmitSemaphores[RHI::MAX_SWAPCHAIN_IMAGES] = {nullptr};
        VkCommandPool m_RenderPool = nullptr;
        VkCommandPool m_ComputePool = nullptr;
        VkCommandPool m_CopyPool = nullptr;
        VkDescriptorPool m_DescriptorPool = nullptr;
        VmaVulkanFunctions* m_VulkanFunctions = nullptr;

        Swapchain m_Swapchain;
        Queue m_RenderQueue{this};
        Queue m_ComputeQueue{this};
        Queue m_CopyQueue{this};
        CommandBuffer m_CmdBuffers[RHI::NUM_FRAMES_IN_FLIGHT];

        uint32_t m_FrameIndex = 0;
        uint32_t m_SwapchainImageIndex = 0;
        uint64_t m_WindowResizeEventId = UINT64_MAX;
        Window* m_Window = nullptr;
    };
}
