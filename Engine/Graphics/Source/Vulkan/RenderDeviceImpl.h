#pragma once
#include "Luma/Rendering/RenderDevice.h"
#include "VulkanFwd.h"

#define VK_FAILED(res) (res != VK_SUCCESS)

namespace luma::vulkan
{
    struct Frame
    {
        VkSemaphore submitSemaphore = nullptr;
        VkSemaphore presentSemaphore = nullptr;
        VkFence fence = nullptr;
    };

    class FRenderDeviceImpl final : public IRenderDevice
    {
    public:
        ERenderDeviceType getDeviceType() override;

        bool initialize(const FRenderDeviceDesc& deviceDesc) override;
        void destroy() override;

        void beginFrame() override;
        void endFrame() override;
        void present() override;
        void waitIdle() override;
        uint32_t getFrameCount() override;
        uint32_t getCurrentFrameIndex() override;
        bool hasVSync() override;

        IBuffer* createBuffer(const FBufferDesc& bufferDesc) override;
        ITexture* createTexture(const FTextureDesc& textureDesc) override;
        IShader* createShader(const FShaderDesc& shaderDesc) override;
        ICommandBuffer* createCommandBuffer(const FCommandBufferDesc& commandBufferDesc) override;
        ISampler* createSampler(const FSamplerDesc& samplerDesc) override;

        IRenderCommandBuffer* createRenderCommandBuffer(const FCommandBufferDesc& cmdBufferDesc) override;
        IComputeCommandBuffer* createComputeCommandBuffer(const FCommandBufferDesc& cmdBufferDesc) override;
        ICopyCommandBuffer* createCopyCommandBuffer(const FCommandBufferDesc& cmdBufferDesc) override;
        IGraphicsPipeline* createGraphicsPipeline(const FGraphicsPipelineDesc& pipelineDesc) override;
        IComputePipeline* createComputePipeline(const FComputePipelineDesc& pipelineDesc) override;
        IFence* createFence(const FFenceDesc& fenceDesc) override;

    private:
        static inline VkInstance s_Instance = nullptr;
        VkPhysicalDevice m_PhysicalDevice = nullptr;
        VkDevice m_Handle = nullptr;
        VmaAllocator m_Allocator = nullptr;
        VkSurfaceKHR m_Surface = nullptr;
        VkCommandPool m_RenderPool = nullptr;
        VkCommandPool m_ComputePool = nullptr;
        VkCommandPool m_CopyPool = nullptr;
    };


}
