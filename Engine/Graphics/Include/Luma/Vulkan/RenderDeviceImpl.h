#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/RenderDevice.h"
#include "SwapchainImpl.h"
#include "QueueImpl.h"
#include "CommandBufferImpl.h"
#include "ImmediateExecutorImpl.h"
#include "VulkanFwd.h"

#define VK_FAILED(res) (res != VK_SUCCESS)

namespace Luma::Vulkan
{
    struct FDescriptorBufferProperties
    {
        uint32_t descriptorBufferOffsetAlignment = 0;
        uint32_t maxDescriptorBufferBindings = 0;
        uint32_t uniformBufferDescriptorSize = 0;
        uint32_t storageBufferDescriptorSize = 0;
        uint32_t sampledImageDescriptorSize = 0;
        uint32_t storageImageDescriptorSize = 0;
        uint32_t samplerDescriptorSize = 0;
        uint32_t combinedImageSamplerDescriptorSize = 0;
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
        uint32_t getFrameCount() const override;
        uint32_t getFrameIndex() const override;

        ISwapchain* getSwapchain() override;
        IQueue* getRenderQueue() override;
        IQueue* getComputeQueue() override;
        IQueue* getCopyQueue() override;

        IBuffer* createBuffer(const FBufferDesc& bufferDesc) override;
        ITexture* createTexture(const FTextureDesc& textureDesc) override;
        ITextureView* createTextureView(const FTextureViewDesc& textureViewDesc) override;
        IShaderProgram* createShader(const FShaderDesc& shaderDesc) override;
        ICommandBuffer* createCommandBuffer(const FCommandBufferDesc& cmdBufferDesc) override;
        ISampler* createSampler(const FSamplerDesc& samplerDesc) override;
        IRenderPipeline* createRenderPipeline(const FRenderPipelineDesc& pipelineDesc) override;
        IComputePipeline* createComputePipeline(const FComputePipelineDesc& pipelineDesc) override;
        IFence* createFence(const FFenceDesc& fenceDesc) override;

        ICommandBuffer* getCommandBuffer() override { return &m_CmdBuffers[m_FrameIndex]; }
        ITextureView* getAcquiredSwapchainTextureView() override;

        void writeSamplerDescriptor(IBuffer* buffer, uint64_t offset, const ISampler* sampler) override;
        void writeTextureDescriptor(IBuffer* buffer, uint64_t offset, const ITexture* texture, ETextureBindingType bindingType) override;
        void writeBufferDescriptor(IBuffer* buffer, uint64_t offset, const IBuffer* bufferResource, uint64_t resourceOffset, uint64_t resourceSize, EBufferBindingType bindingType) override;

        static VkInstance getInstance();
        VkDevice getHandle() const { return m_Handle; }
        VkSurfaceKHR getSurface() const { return m_Surface; }
        VkPhysicalDevice getPhysicalDevice() const { return m_PhysicalDevice; }
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
        VkFence m_Fences[NUM_FRAMES_IN_FLIGHT] = {nullptr};
        VkSemaphore m_TextureAvailableSemaphores[NUM_FRAMES_IN_FLIGHT] = {nullptr};
        VkSemaphore m_SubmitSemaphores[MAX_SWAPCHAIN_IMAGES] = {nullptr};
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
        FCommandBufferImpl m_CmdBuffers[NUM_FRAMES_IN_FLIGHT];
        FDescriptorBufferProperties m_DescriptorBufferProperties;

        uint32_t m_FrameIndex = 0;
        uint32_t m_SwapchainImageIndex = 0;
        uint64_t m_WindowResizeEventId = UINT64_MAX;
        IWindow* m_Window = nullptr;
    };
}
