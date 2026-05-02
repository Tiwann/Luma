#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/RenderDevice.h"
#include "SwapchainImpl.h"
#include "QueueImpl.h"
#include "FenceImpl.h"
#include "SemaphoreImpl.h"
#include "CommandBufferImpl.h"
#include "ImmediateExecutorImpl.h"
#include "VulkanFwd.h"
#include "Luma/Utility/SlangFwd.h"
#define VK_FAILED(res) (res != VK_SUCCESS)

namespace Luma::Vulkan
{
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

        IBuffer* createBuffer(const FBufferDesc& bufferDesc) override;
        ITexture* createTexture(const FTextureDesc& textureDesc) override;
        ITextureView* createTextureView(const FTextureViewDesc& textureViewDesc) override;
        IShader* createShader(const FShaderDesc& shaderDesc) override;
        ICommandBuffer* createCommandBuffer(const FCommandBufferDesc& cmdBufferDesc) override;
        ISampler* createSampler(const FSamplerDesc& samplerDesc) override;
        IGraphicsPipeline* createGraphicsPipeline(const FGraphicsPipelineDesc& pipelineDesc) override;
        IComputePipeline* createComputePipeline(const FComputePipelineDesc& pipelineDesc) override;
        IFence* createFence(const FFenceDesc& fenceDesc) override;
        ISemaphore* createSemaphore(const FSemaphoreDesc& semaphoreDesc) override;

        ICommandBuffer* getCommandBuffer() override { return &m_CmdBuffers[m_CurrentFrameIndex]; }
        ITextureView* getAcquiredSwapchainTextureView() override;

        static VkInstance getInstance();
        VkDevice getHandle() const { return m_Handle; }
        VkSurfaceKHR getSurface() const { return m_Surface; }
        VmaAllocator getAllocator() const { return m_Allocator; }
        VkCommandPool getRenderPool() const { return m_RenderPool; }
        VkCommandPool getComputePool() const { return m_ComputePool; }
        VkCommandPool getCopyPool() const { return m_CopyPool; }
        VkCommandPool getCommandPool(EQueueType queueType) const;
        VkDescriptorPool getDescriptorPool() const { return m_DescriptorPool; }
        FImmediateExecutorImpl& getExecutor();

        slang::IGlobalSession* getSlangSession() const;
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
        VmaVulkanFunctions* m_VulkanFunctions = nullptr;

        FSwapchainImpl m_Swapchain;
        FQueueImpl m_RenderQueue{this};
        FQueueImpl m_ComputeQueue{this};
        FQueueImpl m_CopyQueue{this};
        FImmediateExecutorImpl m_ImmediateExecutor;
        FSemaphoreImpl m_SubmitSemaphores[3];
        FSemaphoreImpl m_PresentSemaphores[3];
        FFenceImpl m_Fences[3];
        FCommandBufferImpl m_CmdBuffers[3];

        uint32_t m_CurrentFrameIndex = 0;
        uint32_t m_SwapchainImageIndex = 0;
        IWindow* m_Window = nullptr;
        slang::IGlobalSession* m_SlangSession = nullptr;
    };
}
