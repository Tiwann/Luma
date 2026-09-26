#include "Luma/Rendering/RenderPassDesc.h"
#include "Luma/Math/Functions.h"
#include "Luma/Math/Rect3.h"
#include "Luma/Vulkan/CommandBuffer.h"
#include "Luma/Vulkan/Device.h"
#include "Luma/Vulkan/Buffer.h"
#include "Luma/Vulkan/Shader.h"
#include "Luma/Vulkan/ComputePipeline.h"
#include "Luma/Vulkan/RenderPipeline.h"
#include "Luma/Vulkan/Conversions.h"
#include "Luma/Vulkan/VulkanUtils.h"
#include "Luma/Vulkan/BindingGroup.h"
#include "Luma/Asset/Material.h"
#include "Luma/Asset/StaticMesh.h"

#include <volk.h>


#define LUMA_CHECK(x, msg) \
    LUMA_ASSERT(x, msg); \
    if(!x) return

namespace Luma::Vulkan
{
    QueueType CommandBuffer::getCommandBufferType()
    {
        return m_CmdBufferType;
    }

    bool CommandBuffer::initialize(const RHI::CommandBufferDesc& cmdBufferDesc)
    {
        if (!cmdBufferDesc.device) return false;

        Device* device = static_cast<Device*>(cmdBufferDesc.device);
        const VkCommandPool commandPool = device->getCommandPool(cmdBufferDesc.queue->getQueueType());
        if (!commandPool) return false;

        VkCommandBufferAllocateInfo info { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
        info.commandPool = commandPool;
        info.commandBufferCount = 1;
        info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

        const VkDevice deviceHandle = device->getHandle();
        if (VK_FAILED(vkAllocateCommandBuffers(deviceHandle, &info, &m_Handle)))
            return false;

        m_Device = device;
        m_PoolHandle = commandPool;
        return true;
    }

    void CommandBuffer::destroy()
    {
        if (!m_Device) return;
        const VkDevice deviceHandle = m_Device->getHandle();
        vkFreeCommandBuffers(deviceHandle, m_PoolHandle, 1, &m_Handle);
        m_Device = nullptr;
        m_PoolHandle = nullptr;
        m_Handle = nullptr;
    }

    void CommandBuffer::reset()
    {
        vkResetCommandBuffer(m_Handle, 0);
    }

    bool CommandBuffer::begin()
    {
        VkCommandBufferBeginInfo info { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
        info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        if (vkBeginCommandBuffer(m_Handle, &info) != VK_SUCCESS)
            return false;
        return true;
    }

    void CommandBuffer::end()
    {
        vkEndCommandBuffer(m_Handle);
    }

    void CommandBuffer::beginDebugGroup(const StringView name, const Color& color)
    {
        VkDebugUtilsLabelEXT labelInfo { VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT };
        labelInfo.pLabelName = *name;
        labelInfo.color[0] = color.r;
        labelInfo.color[1] = color.g;
        labelInfo.color[2] = color.b;
        labelInfo.color[3] = color.a;
        vkCmdBeginDebugUtilsLabelEXT(m_Handle, &labelInfo);
    }

    void CommandBuffer::endDebugGroup()
    {
        vkCmdEndDebugUtilsLabelEXT(m_Handle);
    }

    void CommandBuffer::clearColorTarget(const uint32_t targetIndex, const Color& color)
    {
        VkClearAttachment clearAttachment;
        clearAttachment.colorAttachment = targetIndex;
        clearAttachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        clearAttachment.clearValue.color = VkClearColorValue{{ color.r, color.g, color.b, color.a }};

        VkClearRect clearRect;
        clearRect.rect.offset = VkOffset2D{ static_cast<int32_t>(m_CurrentRenderPassDesc->renderArea.x), static_cast<int32_t>(m_CurrentRenderPassDesc->renderArea.y) };
        clearRect.rect.extent = VkExtent2D{ m_CurrentRenderPassDesc->renderArea.width, m_CurrentRenderPassDesc->renderArea.height };
        clearRect.baseArrayLayer = 0;
        clearRect.layerCount = 1;
        vkCmdClearAttachments(m_Handle, 1, &clearAttachment, 1, &clearRect);
    }

    void CommandBuffer::clearDepthStencilTarget(const float depth, const uint8_t stencil)
    {
        VkClearAttachment clearAttachment;
        clearAttachment.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
        clearAttachment.clearValue.depthStencil = { depth, stencil };

        VkClearRect clearRect;
        clearRect.rect.offset = VkOffset2D{ static_cast<int32_t>(m_CurrentRenderPassDesc->renderArea.x), static_cast<int32_t>(m_CurrentRenderPassDesc->renderArea.y) };
        clearRect.rect.extent = VkExtent2D{ m_CurrentRenderPassDesc->renderArea.width, m_CurrentRenderPassDesc->renderArea.height };
        clearRect.baseArrayLayer = 0;
        clearRect.layerCount = 1;
        vkCmdClearAttachments(m_Handle, 1, &clearAttachment, 1, &clearRect);
    }

    void CommandBuffer::clearColorTexture(RHI::Texture* texture, const Color& color, const TextureSubresourceRange& subresourceRange)
    {
        LUMA_CHECK(texture, "Invalid texture handle!");
        Texture* textureImpl = static_cast<Texture*>(texture);
        const VkClearColorValue clearColor = VkClearColorValue{{ color.r, color.g, color.b, color.a }};
        const VkImageSubresourceRange range = convert<VkImageSubresourceRange>(subresourceRange);
        vkCmdClearColorImage(m_Handle, textureImpl->getImage(),  convert<VkImageLayout>(texture->getResourceState()), &clearColor, 1, &range);
    }

    void CommandBuffer::clearColorTexture(RHI::Texture* texture, const Color& color)
    {
        LUMA_CHECK(texture, "Invalid texture handle!");
        Texture* textureImpl = static_cast<Texture*>(texture);
        const VkClearColorValue clearColor = VkClearColorValue{{ color.r, color.g, color.b, color.a }};

        VkImageSubresourceRange range;
        range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        range.baseMipLevel = 0;
        range.levelCount = textureImpl->getMipCount();
        range.baseArrayLayer = 0;
        range.layerCount = textureImpl->getArrayCount();
        vkCmdClearColorImage(m_Handle, textureImpl->getImage(), convert<VkImageLayout>(texture->getResourceState()), &clearColor, 1, &range);
    }

    void CommandBuffer::bindVertexBuffers(ArrayView<VertexBufferBinding> bindings)
    {
        if (bindings.isEmpty()) return;
        Array<VkBuffer> buffers;
        Array<VkDeviceSize> offsets;
        for (const VertexBufferBinding& binding : bindings)
        {
            buffers.add(static_cast<const Buffer*>(binding.buffer)->getHandle());
            offsets.add(binding.offset);
        }

        vkCmdBindVertexBuffers(m_Handle, 0, bindings.count(), buffers.data(), offsets.data());
    }

    void CommandBuffer::bindIndexBuffer(const RHI::Buffer* buffer, int64_t offset, const IndexFormat format)
    {
        if (!buffer) return;
        const Buffer* bufferImpl = static_cast<const Buffer*>(buffer);
        const VkBuffer bufferHandle = bufferImpl->getHandle();
        const VkDeviceSize size = bufferImpl->getSize();

        vkCmdBindIndexBuffer2(m_Handle, bufferHandle, offset, size, convert<VkIndexType>(format));
    }

    void CommandBuffer::pushConstants(const RHI::Shader* shader, ShaderStageFlags stageFlags, const void* data, uint64_t offset, uint64_t size)
    {
        const Shader* shaderImpl = static_cast<const Shader*>(shader);
        vkCmdPushConstants(m_Handle, shaderImpl->getPipelineLayout(), convert<VkShaderStageFlags>(stageFlags), offset, size, data);
    }

    void CommandBuffer::bindRenderPipeline(const RHI::RenderPipeline* pipeline)
    {
        if (!pipeline) return;
        const RenderPipeline* pipelineImpl = static_cast<const RenderPipeline*>(pipeline);
        vkCmdBindPipeline(m_Handle, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineImpl->getHandle());
    }

    static VkRenderingAttachmentInfo getVulkanRenderingAttachmentInfo(const RHI::RenderPassTarget& attachment)
    {
        const auto* textureViewImpl = static_cast<const TextureView*>(attachment.textureView);
        const auto* resolveTextureView = static_cast<const TextureView*>(attachment.resolveTextureView);

        VkRenderingAttachmentInfo info { VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO };
        info.loadOp = convert<VkAttachmentLoadOp>(attachment.loadOp);
        info.storeOp = convert<VkAttachmentStoreOp>(attachment.storeOp);
        info.imageView = textureViewImpl->getHandle();
        info.imageLayout = convert<VkImageLayout>(attachment.type);

        switch (attachment.type)
        {
        case RHI::RenderPassTargetType::Color:
            {
                const Color& clearColor = attachment.clearValue.color;
                info.clearValue.color = { clearColor.r, clearColor.g, clearColor.b, clearColor.a };
            }
            break;
        case RHI::RenderPassTargetType::DepthStencil:
            {
                const RHI::ClearValue& clearValue = attachment.clearValue;
                info.clearValue.depthStencil = { clearValue.depth, clearValue.stencil };
            }
            break;
        }

        if (resolveTextureView)
        {
            info.resolveMode = convert<VkResolveModeFlagBits>(attachment.resolveMode);
            info.resolveImageLayout = convert<VkImageLayout>(attachment.type);
            info.resolveImageView = resolveTextureView->getHandle();
        }

        return info;
    }

    void CommandBuffer::beginRenderPass(const RHI::RenderPassDesc& renderPassDesc)
    {
        Array<VkRenderingAttachmentInfo> colorAttachments;
        VkRenderingAttachmentInfo depthStencilAttachment;
        for (const auto* attachment : renderPassDesc.colorTargets)
            if (attachment) colorAttachments.add(getVulkanRenderingAttachmentInfo(*attachment));

        VkRenderingInfo renderingInfo { VK_STRUCTURE_TYPE_RENDERING_INFO };
        renderingInfo.layerCount = 1;
        renderingInfo.viewMask = 0;
        renderingInfo.renderArea.extent = { renderPassDesc.renderArea.width, renderPassDesc.renderArea.height };
        renderingInfo.renderArea.offset = { (int32_t)renderPassDesc.renderArea.x, (int32_t)renderPassDesc.renderArea.y };
        renderingInfo.pColorAttachments = colorAttachments.data();
        renderingInfo.colorAttachmentCount = colorAttachments.count();

        if (renderPassDesc.depthStencilTarget)
        {
            depthStencilAttachment = getVulkanRenderingAttachmentInfo(*renderPassDesc.depthStencilTarget);
            renderingInfo.pDepthAttachment = &depthStencilAttachment;
            renderingInfo.pStencilAttachment = &depthStencilAttachment;
        }

        vkCmdBeginRendering(m_Handle, &renderingInfo);
        m_CurrentRenderPassDesc = &renderPassDesc;
    }

    void CommandBuffer::endRenderPass()
    {
        vkCmdEndRendering(m_Handle);
        m_CurrentRenderPassDesc = nullptr;
    }

    void CommandBuffer::setViewports(const Array<Viewport>& viewports)
    {
        Array<VkViewport> vulkanViewports = viewports.transform<VkViewport>([](const Viewport& v)
        {
            VkViewport vp { };
            vp.x = v.x;
            vp.y = v.y + v.height;
            vp.width = v.width;
            vp.height = -v.height;
            vp.minDepth = v.minDepth;
            vp.maxDepth = v.maxDepth;
            return vp;
        });

        vkCmdSetViewport(m_Handle, 0, vulkanViewports.count(),  vulkanViewports.data());
    }

    void CommandBuffer::setScissors(const Array<Scissor>& scissors)
    {
        Array<VkRect2D> vulkanScissors = scissors.transform<VkRect2D>([](const Scissor& s)
        {
            VkRect2D rect { };
            rect.offset.x = s.x;
            rect.offset.y = s.y;
            rect.extent.width = s.width;
            rect.extent.height = s.height;
            return rect;
        });

        vkCmdSetScissor(m_Handle, 0, vulkanScissors.count(), vulkanScissors.data());
    }

    void CommandBuffer::setViewport(const Viewport& viewport)
    {
        VkViewport vp { };
        vp.x = viewport.x;
        vp.y = viewport.y + viewport.height;
        vp.width = viewport.width;
        vp.height = -viewport.height;
        vp.minDepth = viewport.minDepth;
        vp.maxDepth = viewport.maxDepth;
        vkCmdSetViewport(m_Handle, 0, 1, &vp);
    }

    void CommandBuffer::setScissor(const Scissor& scissor)
    {
        VkRect2D rect { };
        rect.offset.x = scissor.x;
        rect.offset.y = scissor.y;
        rect.extent.width = scissor.width;
        rect.extent.height = scissor.height;
        vkCmdSetScissor(m_Handle, 0, 1, &rect);
    }

    void CommandBuffer::draw(const DrawCommand& drawCmd)
    {
        vkCmdDraw(m_Handle,
            drawCmd.vertexCount,
            drawCmd.instanceCount,
            drawCmd.firstVertex,
            drawCmd.firstInstance);
    }

    void CommandBuffer::drawIndexed(const DrawIndexedCommand& drawIndexedCmd)
    {
        vkCmdDrawIndexed(m_Handle,
            drawIndexedCmd.indexCount,
            drawIndexedCmd.instanceCount,
            drawIndexedCmd.firstIndex,
            drawIndexedCmd.vertexOffset,
            drawIndexedCmd.firstInstance);
    }

    void CommandBuffer::drawIndirect(const RHI::Buffer* buffer, const uint64_t offset, const uint32_t drawCount)
    {
        LUMA_CHECK(buffer, "Invalid buffer handle!");
        const Buffer* bufferImpl = static_cast<const Buffer*>(buffer);
        vkCmdDrawIndirect(m_Handle, bufferImpl->getHandle(), offset, drawCount, sizeof(DrawCommand));
    }

    void CommandBuffer::drawIndexedIndirect(const RHI::Buffer* buffer, const uint64_t offset, const uint32_t drawCount)
    {
        LUMA_CHECK(buffer, "Invalid buffer handle!");
        const Buffer* bufferImpl = static_cast<const Buffer*>(buffer);
        vkCmdDrawIndexedIndirect(m_Handle, bufferImpl->getHandle(), offset, drawCount, sizeof(DrawIndexedCommand));
    }

    void CommandBuffer::bindComputePipeline(const RHI::ComputePipeline* pipeline)
    {
        if (!pipeline) return;
        const ComputePipeline* pipelineImpl = static_cast<const ComputePipeline*>(pipeline);
        vkCmdBindPipeline(m_Handle, VK_PIPELINE_BIND_POINT_COMPUTE, pipelineImpl->getHandle());
    }

    void CommandBuffer::dispatch(const uint32_t groupCountX, const uint32_t groupCountY, const uint32_t groupCountZ)
    {
        vkCmdDispatch(m_Handle, groupCountX, groupCountY, groupCountZ);
    }

    void CommandBuffer::dispatchIndirect(RHI::Buffer* buffer, const int64_t offset)
    {
        LUMA_CHECK(buffer, "Invalid buffer handle!");
        const Buffer* bufferImpl = static_cast<const Buffer*>(buffer);
        vkCmdDispatchIndirect(m_Handle, bufferImpl->getHandle(), offset);
    }

    void CommandBuffer::copyBuffer(RHI::Buffer* srcBuffer, RHI::Buffer* dstBuffer, const int64_t srcOffset, const int64_t dstOffset, const uint64_t size)
    {
        LUMA_CHECK(srcBuffer, "Invalid buffer handle!");
        LUMA_CHECK(dstBuffer, "Invalid buffer handle!");

        VkBufferCopy2 region = { VK_STRUCTURE_TYPE_BUFFER_COPY_2 };
        region.srcOffset = srcOffset;
        region.dstOffset = dstOffset;
        region.size = size;

        const Buffer* srcBufferImpl = static_cast<const Buffer*>(srcBuffer);
        const Buffer* dstBufferImpl = static_cast<const Buffer*>(dstBuffer);
        VkCopyBufferInfo2 copyInfo { VK_STRUCTURE_TYPE_COPY_BUFFER_INFO_2 };
        copyInfo.srcBuffer = srcBufferImpl->getHandle();
        copyInfo.dstBuffer = dstBufferImpl->getHandle();
        copyInfo.regionCount = 1;
        copyInfo.pRegions = &region;
        vkCmdCopyBuffer2(m_Handle, &copyInfo);
    }

    void CommandBuffer::copyBufferToTexture(RHI::Buffer* buffer, const int64_t offset, uint64_t size, RHI::Texture* texture, const uint32_t arrayIndex, const uint32_t mipLevel)
    {
        LUMA_CHECK(buffer, "Invalid buffer handle!");
        LUMA_CHECK(texture, "Invalid texture handle!");

        const Buffer* bufferImpl = static_cast<const Buffer*>(buffer);
        const Texture* textureImpl = static_cast<const Texture*>(texture);

        const uint32_t mipWidth = max(1u, textureImpl->getWidth() >> mipLevel);
        const uint32_t mipHeight = max(1u, textureImpl->getHeight() >> mipLevel);
        const uint32_t mipDepth = max(1u, textureImpl->getDepth() >> mipLevel);

        VkBufferImageCopy2 region = { VK_STRUCTURE_TYPE_BUFFER_IMAGE_COPY_2 };
        region.bufferOffset = offset;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // ASSUMING COPYING ONLY COLOR TEXTURES
        region.imageSubresource.mipLevel = mipLevel;
        region.imageSubresource.baseArrayLayer = arrayIndex;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = { mipWidth, mipHeight, mipDepth };

        VkCopyBufferToImageInfo2 copyInfo = { VK_STRUCTURE_TYPE_COPY_BUFFER_TO_IMAGE_INFO_2 };
        copyInfo.srcBuffer = bufferImpl->getHandle();
        copyInfo.dstImage = textureImpl->getImage();
        copyInfo.dstImageLayout = convert<VkImageLayout>(textureImpl->getResourceState());
        copyInfo.regionCount = 1;
        copyInfo.pRegions = &region;

        vkCmdCopyBufferToImage2(m_Handle, &copyInfo);
    }

    void CommandBuffer::blitTexture(const RHI::Texture* srcTexture, const FRect3u& srcRect,
                                    uint32_t srcMipLevel, uint32_t srcBaseArrayLayer, uint32_t srcArrayCount,
                                    const RHI::Texture* destTexture, const FRect3u& destRect, const TextureSubresourceRange& destRange, uint32_t
                                    destBaseArrayLayer, uint32_t destArrayCount, Filter filter, uint32_t destMipLevel)
    {
        const Texture* srcTextureImpl = static_cast<const Texture*>(srcTexture);
        const Texture* destTextureImpl = static_cast<const Texture*>(destTexture);


        VkImageBlit2 region{VK_STRUCTURE_TYPE_IMAGE_BLIT_2};
        region.srcOffsets[0] = VkOffset3D(srcRect.x, srcRect.y, srcRect.z);
        region.srcOffsets[1] = VkOffset3D(srcRect.width, srcRect.height, srcRect.depth);
        region.dstOffsets[0] = VkOffset3D(destRect.x, destRect.y, destRect.z);
        region.dstOffsets[1] = VkOffset3D(destRect.width, destRect.height, destRect.depth);
        region.srcSubresource.aspectMask = convert<VkImageAspectFlags>(srcTexture->getUsageFlags());
        region.srcSubresource.mipLevel = srcMipLevel;
        region.srcSubresource.baseArrayLayer = srcBaseArrayLayer;
        region.srcSubresource.layerCount = srcArrayCount;
        region.dstSubresource.aspectMask = convert<VkImageAspectFlags>(destTexture->getUsageFlags());
        region.dstSubresource.mipLevel = destMipLevel;
        region.dstSubresource.baseArrayLayer = destBaseArrayLayer;
        region.dstSubresource.layerCount = destArrayCount;

        VkBlitImageInfo2 blitInfo {VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2};
        blitInfo.srcImage = srcTextureImpl->getImage();
        blitInfo.srcImageLayout = convert<VkImageLayout>(srcTextureImpl->getResourceState());
        blitInfo.dstImage = destTextureImpl->getImage();
        blitInfo.dstImageLayout = convert<VkImageLayout>(destTextureImpl->getResourceState());
        blitInfo.filter = convert<VkFilter>(filter);
        blitInfo.pRegions = &region;
        blitInfo.regionCount = 1;

        vkCmdBlitImage2(m_Handle, &blitInfo);
    }

    void CommandBuffer::textureBarriers(ArrayView<TextureBarrier> barriers)
    {
        Array<VkImageMemoryBarrier2> imageBarriers;
        for (const TextureBarrier& barrier : barriers)
            imageBarriers.add(makeTextureBarrier(barrier));

        VkDependencyInfo dependencyInfo{VK_STRUCTURE_TYPE_DEPENDENCY_INFO};
        dependencyInfo.pImageMemoryBarriers = imageBarriers.data();
        dependencyInfo.imageMemoryBarrierCount = imageBarriers.count();
        vkCmdPipelineBarrier2(m_Handle, &dependencyInfo);

        for (const TextureBarrier& barrier : barriers)
        {
            Texture* texture = static_cast<Texture*>(barrier.texture);
            texture->setResourceState(barrier.destState);
        }
    }

    void CommandBuffer::bufferBarriers(ArrayView<BufferBarrier> barriers)
    {
        Array<VkBufferMemoryBarrier2> memoryBarriers;
        for (const BufferBarrier& barrier : barriers)
            memoryBarriers.add(makeBufferBarrier(barrier));

        VkDependencyInfo dependencyInfo{VK_STRUCTURE_TYPE_DEPENDENCY_INFO};
        dependencyInfo.pBufferMemoryBarriers = memoryBarriers.data();
        dependencyInfo.bufferMemoryBarrierCount = memoryBarriers.count();
        vkCmdPipelineBarrier2(m_Handle, &dependencyInfo);

        for (const BufferBarrier& barrier : barriers)
        {
            Buffer* buffer = static_cast<Buffer*>(barrier.buffer);
            buffer->setResourceState(barrier.destState);
        }
    }

    void CommandBuffer::bindBindingGroup(const RHI::BindingGroup* bindingGroup)
    {
        const BindingGroup* bindingGroupImpl = static_cast<const BindingGroup*>(bindingGroup);
        const VkDescriptorSet descriptorSet = bindingGroupImpl->getDescriptorSet();
        const Shader* shaderImpl = static_cast<const Shader*>(bindingGroupImpl->getShader());
        const ShaderStageFlags stageFlags = shaderImpl->getStages();
        const VkPipelineLayout pipelineLayout = shaderImpl->getPipelineLayout();

        VkBindDescriptorSetsInfo bindInfo{VK_STRUCTURE_TYPE_BIND_DESCRIPTOR_SETS_INFO};
        bindInfo.layout = pipelineLayout;
        bindInfo.firstSet = bindingGroupImpl->getGroupIndex();
        bindInfo.descriptorSetCount = 1;
        bindInfo.pDescriptorSets = &descriptorSet;
        bindInfo.stageFlags = convert<VkShaderStageFlags>(stageFlags);

        vkCmdBindDescriptorSets2(m_Handle, &bindInfo);
    }

    void CommandBuffer::bindDescriptorBuffer(const RHI::Buffer* buffer)
    {
        VkDescriptorBufferBindingInfoEXT bindingInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_BUFFER_BINDING_INFO_EXT};
        bindingInfo.address = buffer->getDeviceAddress();
        bindingInfo.usage = VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT | VK_BUFFER_USAGE_SAMPLER_DESCRIPTOR_BUFFER_BIT_EXT;
        vkCmdBindDescriptorBuffersEXT(m_Handle, 1, &bindingInfo);
    }

    void CommandBuffer::setName(const StringView name)
    {
        setVulkanObjectDebugName(m_Device, VK_OBJECT_TYPE_COMMAND_BUFFER, m_Handle, name);
    }
}
