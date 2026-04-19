#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/RenderDevice.h"
#include "SwapchainImpl.h"
#include "QueueImpl.h"
#include "FenceImpl.h"
#include "CommandBufferImpl.h"
#include "ImmediateExecutorImpl.h"
#include "VulkanFwd.h"

#define VK_FAILED(res) (res != VK_SUCCESS)

namespace Luma::Vulkan
{
    struct FFrame
    {
        VkSemaphore submitSemaphore = nullptr;
        VkSemaphore presentSemaphore = nullptr;
        FFenceImpl fence;
        FCommandBufferImpl cmdBuffer;
    };

    class LUMA_GRAPHICS_API FRenderDeviceImpl final : public IRenderDevice
    {
    public:
        ERenderDeviceType getDeviceType() override;

        bool initialize(const FRenderDeviceDesc& deviceDesc) override;
        void destroy() override;

        bool beginFrame() override;
        void endFrame() override;
        void present() override;
        void waitIdle() override;
        uint32_t getFrameCount() override;
        uint32_t getCurrentFrameIndex() override;
        bool hasVSync() override;

        ISwapchain* getSwapchain() override;
        IQueue* getRenderQueue() override;
        IQueue* getComputeQueue() override;
        IQueue* getCopyQueue() override;

        IBuffer* createBuffer(const FBufferDesc& bufferDesc) const override;
        ITexture* createTexture(const FTextureDesc& textureDesc) const override;
        ITextureView* createTextureView(const FTextureViewDesc& textureViewDesc) const override;
        IShader* createShader(const FShaderDesc& shaderDesc) const override;
        ICommandBuffer* createCommandBuffer(const FCommandBufferDesc& cmdBufferDesc) const override;
        ISampler* createSampler(const FSamplerDesc& samplerDesc) const override;
        ISampler* getOrCreateSampler(const FSamplerDesc& samplerDesc) override;
        IGraphicsPipeline* createGraphicsPipeline(const FGraphicsPipelineDesc& pipelineDesc) const override;
        IComputePipeline* createComputePipeline(const FComputePipelineDesc& pipelineDesc) const override;
        IFence* createFence(const FFenceDesc& fenceDesc) const override;

        ICommandBuffer* getCurrentCommandBuffer() { return &m_Frames[m_CurrentFrameIndex].cmdBuffer; }

        static VkInstance getInstance() { return s_Instance; }
        VkDevice getHandle() const { return m_Handle; }
        VkSurfaceKHR getSurface() const { return m_Surface; }
        VmaAllocator getAllocator() const { return m_Allocator; }
        VkCommandPool getRenderPool() const { return m_RenderPool; }
        VkCommandPool getComputePool() const { return m_ComputePool; }
        VkCommandPool getCopyPool() const { return m_CopyPool; }
        VkCommandPool getCommandPool(EQueueType queueType) const;
        VkDescriptorPool getDescriptorPool() const { return m_DescriptorPool; }
        FImmediateExecutorImpl& getExecutor();

    private:
        static inline VkInstance s_Instance = nullptr;
        static inline uint32_t s_DeviceCount = 0;
#if defined(LUMA_DEBUG)
        static inline VkDebugUtilsMessengerEXT s_DebugMessenger = nullptr;
#endif
        VkPhysicalDevice m_PhysicalDevice = nullptr;
        VkDevice m_Handle = nullptr;
        VmaAllocator m_Allocator = nullptr;
        VkSurfaceKHR m_Surface = nullptr;
        VkCommandPool m_RenderPool = nullptr;
        VkCommandPool m_ComputePool = nullptr;
        VkCommandPool m_CopyPool = nullptr;
        VkDescriptorPool m_DescriptorPool = nullptr;
        VmaVulkanFunctions* m_VkProcs = nullptr;

        FSwapchainImpl m_Swapchain;
        FQueueImpl m_RenderQueue;
        FQueueImpl m_ComputeQueue;
        FQueueImpl m_CopyQueue;
        FImmediateExecutorImpl m_ImmediateExecutor;
        FFrame m_Frames[3]{};

        uint32_t m_CurrentFrameIndex = 0;
        uint32_t m_LastFrameIndex = 0;

        IWindow* m_Window = nullptr;
    };
}
