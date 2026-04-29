#include "CommandBufferImpl.h"
#include "RenderDeviceImpl.h"
#include "BufferImpl.h"
#include "ComputePipelineImpl.h"
#include "GraphicsPipelineImpl.h"
#include "Luma/Math/Functions.h"
#include "Luma/Rendering/RenderPassDesc.h"

#include "Conversions.h"
#include "VulkanUtils.h"
#include <Volk/volk.h>

#include "BindingSetImpl.h"
#include "ShaderImpl.h"
#include "Luma/Asset/Material.h"
#include "Luma/Asset/StaticMesh.h"


#define LUMA_CHECK(x, msg) \
    LUMA_ASSERT(x, msg); \
    if(!x) return

namespace Luma::Vulkan
{
    EQueueType FCommandBufferImpl::getCommandBufferType()
    {
        return m_CmdBufferType;
    }

    bool FCommandBufferImpl::initialize(const FCommandBufferDesc& cmdBufferDesc)
    {
        if (!cmdBufferDesc.device) return false;

        FRenderDeviceImpl* device = static_cast<FRenderDeviceImpl*>(cmdBufferDesc.device);
        const VkCommandPool commandPool = device->getCommandPool(cmdBufferDesc.queueType);
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

    void FCommandBufferImpl::destroy()
    {
        if (!m_Device) return;
        const VkDevice deviceHandle = m_Device->getHandle();
        vkFreeCommandBuffers(deviceHandle, m_PoolHandle, 1, &m_Handle);
        m_Device = nullptr;
        m_PoolHandle = nullptr;
        m_Handle = nullptr;
    }

    void FCommandBufferImpl::reset()
    {
        vkResetCommandBuffer(m_Handle, 0);
    }

    bool FCommandBufferImpl::begin()
    {
        VkCommandBufferBeginInfo info { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
        info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        if (vkBeginCommandBuffer(m_Handle, &info) != VK_SUCCESS)
            return false;
        return true;
    }

    void FCommandBufferImpl::end()
    {
        vkEndCommandBuffer(m_Handle);
    }

    void FCommandBufferImpl::beginDebugGroup(const FStringView name, const FColor& color)
    {
        VkDebugUtilsLabelEXT labelInfo { VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT };
        labelInfo.pLabelName = *name;
        labelInfo.color[0] = color.r;
        labelInfo.color[1] = color.g;
        labelInfo.color[2] = color.b;
        labelInfo.color[3] = color.a;
        vkCmdBeginDebugUtilsLabelEXT(m_Handle, &labelInfo);
    }

    void FCommandBufferImpl::endDebugGroup()
    {
        vkCmdEndDebugUtilsLabelEXT(m_Handle);
    }

    void FCommandBufferImpl::clearColor(const uint32_t attachmentIndex, const FColor& color)
    {
        VkClearAttachment clearAttachment;
        clearAttachment.colorAttachment = attachmentIndex;
        clearAttachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        clearAttachment.clearValue.color = VkClearColorValue{{ color.r, color.g, color.b, color.a }};

        VkClearRect clearRect;
        clearRect.rect.offset = VkOffset2D{ static_cast<int32_t>(m_CurrentRenderPassDesc->renderArea.x), static_cast<int32_t>(m_CurrentRenderPassDesc->renderArea.y) };
        clearRect.rect.extent = VkExtent2D{ m_CurrentRenderPassDesc->renderArea.width, m_CurrentRenderPassDesc->renderArea.height };
        clearRect.baseArrayLayer = 0;
        clearRect.layerCount = 1;
        vkCmdClearAttachments(m_Handle, 1, &clearAttachment, 1, &clearRect);
    }

    void FCommandBufferImpl::clearDepthStencil(const float depth, const uint8_t stencil)
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

    void FCommandBufferImpl::clearColorTexture(ITexture* texture, const FColor& color, const FTextureSubresourceRange& subresourceRange)
    {
        LUMA_CHECK(texture, "Invalid texture handle!");
        FTextureImpl* textureImpl = static_cast<FTextureImpl*>(texture);
        const VkClearColorValue clearColor = VkClearColorValue{{ color.r, color.g, color.b, color.a }};
        const VkImageSubresourceRange range = convert<VkImageSubresourceRange>(subresourceRange);
        vkCmdClearColorImage(m_Handle, textureImpl->getImage(),  convert<VkImageLayout>(texture->getResourceState()), &clearColor, 1, &range);
    }

    void FCommandBufferImpl::clearColorTexture(ITexture* texture, const FColor& color)
    {
        LUMA_CHECK(texture, "Invalid texture handle!");
        FTextureImpl* textureImpl = static_cast<FTextureImpl*>(texture);
        const VkClearColorValue clearColor = VkClearColorValue{{ color.r, color.g, color.b, color.a }};

        VkImageSubresourceRange range;
        range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        range.baseMipLevel = 0;
        range.levelCount = textureImpl->getMipCount();
        range.baseArrayLayer = 0;
        range.layerCount = textureImpl->getArrayCount();
        vkCmdClearColorImage(m_Handle, textureImpl->getImage(), convert<VkImageLayout>(texture->getResourceState()), &clearColor, 1, &range);
    }

    void FCommandBufferImpl::bindVertexBuffer(const IBuffer* buffer, const int64_t offset)
    {
        if (!buffer) return;
        const VkDeviceSize offsets[] { static_cast<VkDeviceSize>(offset) };
        const VkBuffer bufferImpl = static_cast<const FBufferImpl*>(buffer)->getHandle();
        const VkBuffer buffers[] { bufferImpl };
        vkCmdBindVertexBuffers(m_Handle, 0, 1, buffers, offsets);
    }

    void FCommandBufferImpl::bindIndexBuffer(const IBuffer* buffer, const uint64_t offset, const EIndexFormat format)
    {
        if (!buffer) return;
        const FBufferImpl* bufferImpl = static_cast<const FBufferImpl*>(buffer);
        const VkBuffer bufferHandle = bufferImpl->getHandle();
        const VkDeviceSize size = bufferImpl->getSize();

        vkCmdBindIndexBuffer2(m_Handle, bufferHandle, offset, size, convert<VkIndexType>(format));
    }

    void FCommandBufferImpl::bindGraphicsPipeline(const IGraphicsPipeline* pipeline)
    {
        if (!pipeline) return;
        const FGraphicsPipelineImpl* pipelineImpl = static_cast<const FGraphicsPipelineImpl*>(pipeline);
        vkCmdBindPipeline(m_Handle, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineImpl->getHandle());
    }

    static VkRenderingAttachmentInfo getVulkanRenderingAttachmentInfo(const FRenderPassAttachment& attachment)
    {
        const auto* textureViewImpl = static_cast<const FTextureViewImpl*>(attachment.textureView);
        const auto* resolveTextureView = static_cast<const FTextureViewImpl*>(attachment.resolveTextureView);

        VkRenderingAttachmentInfo info { VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO };
        info.loadOp = convert<VkAttachmentLoadOp>(attachment.loadOp);
        info.storeOp = convert<VkAttachmentStoreOp>(attachment.storeOp);
        info.imageView = textureViewImpl->getHandle();
        info.imageLayout = convert<VkImageLayout>(attachment.type);

        switch (attachment.type)
        {
        case ERenderPassAttachmentType::Color:
            {
                const FColor& clearColor = attachment.clearValue.color;
                info.clearValue.color = { clearColor.r, clearColor.g, clearColor.b, clearColor.a };
            }
            break;
        case ERenderPassAttachmentType::DepthStencil:
            {
                const FClearValue& clearValue = attachment.clearValue;
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

    void FCommandBufferImpl::beginRenderPass(const FRenderPassDesc& renderPassDesc)
    {
        TArray<VkRenderingAttachmentInfo> colorAttachments;
        VkRenderingAttachmentInfo depthStencilAttachment;
        for (const auto* attachment : renderPassDesc.colorAttachments)
            if (attachment) colorAttachments.add(getVulkanRenderingAttachmentInfo(*attachment));

        VkRenderingInfo renderingInfo { VK_STRUCTURE_TYPE_RENDERING_INFO };
        renderingInfo.layerCount = 1;
        renderingInfo.viewMask = 0;
        renderingInfo.renderArea.extent = { renderPassDesc.renderArea.width, renderPassDesc.renderArea.height };
        renderingInfo.renderArea.offset = { (int32_t)renderPassDesc.renderArea.x, (int32_t)renderPassDesc.renderArea.y };
        renderingInfo.pColorAttachments = colorAttachments.data();
        renderingInfo.colorAttachmentCount = colorAttachments.count();

        if (renderPassDesc.depthStencilAttachment)
        {
            depthStencilAttachment = getVulkanRenderingAttachmentInfo(*renderPassDesc.depthStencilAttachment);
            renderingInfo.pDepthAttachment = &depthStencilAttachment;
            renderingInfo.pStencilAttachment = &depthStencilAttachment;
        }

        vkCmdBeginRendering(m_Handle, &renderingInfo);
        m_CurrentRenderPassDesc = &renderPassDesc;
    }

    void FCommandBufferImpl::endRenderPass()
    {
        vkCmdEndRendering(m_Handle);
        m_CurrentRenderPassDesc = nullptr;
    }

    void FCommandBufferImpl::setViewport(const FViewport& viewport)
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

    void FCommandBufferImpl::setScissor(const FScissor& scissor)
    {
        VkRect2D rect { };
        rect.offset.x = scissor.x;
        rect.offset.y = scissor.y;
        rect.extent.width = scissor.width;
        rect.extent.height = scissor.height;
        vkCmdSetScissor(m_Handle, 0, 1, &rect);
    }

    void FCommandBufferImpl::draw(const FDrawCommand& drawCmd)
    {
        vkCmdDraw(m_Handle,
            drawCmd.vertexCount,
            drawCmd.instanceCount,
            drawCmd.firstVertex,
            drawCmd.firstInstance);
    }

    void FCommandBufferImpl::drawIndexed(const FDrawIndexedCommand& drawIndexedCmd)
    {
        vkCmdDrawIndexed(m_Handle,
            drawIndexedCmd.indexCount,
            drawIndexedCmd.instanceCount,
            drawIndexedCmd.firstIndex,
            drawIndexedCmd.vertexOffset,
            drawIndexedCmd.firstInstance);
    }

    void FCommandBufferImpl::drawIndirect(const IBuffer* buffer, const uint64_t offset, const uint32_t drawCount)
    {
        LUMA_CHECK(buffer, "Invalid buffer handle!");
        const FBufferImpl* bufferImpl = static_cast<const FBufferImpl*>(buffer);
        vkCmdDrawIndirect(m_Handle, bufferImpl->getHandle(), offset, drawCount, sizeof(FDrawCommand));
    }

    void FCommandBufferImpl::drawIndexedIndirect(const IBuffer* buffer, const uint64_t offset, const uint32_t drawCount)
    {
        LUMA_CHECK(buffer, "Invalid buffer handle!");
        const FBufferImpl* bufferImpl = static_cast<const FBufferImpl*>(buffer);
        vkCmdDrawIndexedIndirect(m_Handle, bufferImpl->getHandle(), offset, drawCount, sizeof(FDrawIndexedCommand));
    }

    void FCommandBufferImpl::bindComputePipeline(const IComputePipeline* pipeline)
    {
        if (!pipeline) return;
        const FComputePipelineImpl* pipelineImpl = static_cast<const FComputePipelineImpl*>(pipeline);
        vkCmdBindPipeline(m_Handle, VK_PIPELINE_BIND_POINT_COMPUTE, pipelineImpl->getHandle());
    }

    void FCommandBufferImpl::bindMaterial(const FMaterial* material)
    {
        LUMA_CHECK(material, "Invalid material handle!");
        bindBindingSet(material->getBindingSet(), material->getShader());
    }

    void FCommandBufferImpl::bindBindingSet(const IBindingSet* bindingSet, const IShader* shader)
    {
        LUMA_CHECK(bindingSet, "Invalid binding set handle!");
        LUMA_CHECK(shader, "Invalid shader handle!");

        const FShaderImpl* shaderImpl = static_cast<const FShaderImpl*>(shader);
        const FBindingSetImpl* bindingSetImpl = static_cast<const FBindingSetImpl*>(bindingSet);

        const VkDescriptorSet descriptorSets[] { bindingSetImpl->getHandle() };
        VkBindDescriptorSetsInfo info = { VK_STRUCTURE_TYPE_BIND_DESCRIPTOR_SETS_INFO };
        info.layout = shaderImpl->getPipelineLayout();
        info.firstSet = bindingSetImpl->getSetIndex();
        info.descriptorSetCount = 1;
        info.pDescriptorSets = descriptorSets;
        info.stageFlags = convert<VkShaderStageFlags>(shaderImpl->getStageFlags());
        info.dynamicOffsetCount = 0;
        info.pDynamicOffsets = nullptr;
        vkCmdBindDescriptorSets2(m_Handle, &info);
    }

    void FCommandBufferImpl::drawStaticMesh(const FStaticMesh* staticMesh, const FMaterial* material, const FMatrix4f& transform, const FCameraf& camera)
    {
        LUMA_CHECK(staticMesh, "Invalid static mesh handle!");
        LUMA_CHECK(material, "Invalid material handle!");

        const auto vertexBuffer = staticMesh->getVertexBuffer();
        const auto indexBuffer = staticMesh->getIndexBuffer();
        LUMA_CHECK(vertexBuffer, "Invalid vertex buffer handle!");
        LUMA_CHECK(indexBuffer, "Invalid index buffer handle!");

        bindMaterial(material);
        for (const auto& perMaterialMeshParts : staticMesh->getPerMaterialMeshParts())
        {
            for (const FMeshPart& meshPart : perMaterialMeshParts.value)
            {
                bindVertexBuffer(vertexBuffer, meshPart.vertexOffset);
                bindIndexBuffer(indexBuffer, meshPart.indexOffset, EIndexFormat::Uint32);
                drawIndexed(FDrawIndexedCommand(meshPart.indexSize / sizeof(uint32_t), 1, 0, 0, 0));
            }
        }
    }

    void FCommandBufferImpl::drawStaticMesh(const FStaticMesh* staticMesh, const FMatrix4f& transform, const FCameraf& camera)
    {
        LUMA_CHECK(staticMesh, "Invalid static mesh handle!");

        const auto vertexBuffer = staticMesh->getVertexBuffer();
        const auto indexBuffer = staticMesh->getIndexBuffer();
        LUMA_CHECK(vertexBuffer, "Invalid vertex buffer handle!");
        LUMA_CHECK(indexBuffer, "Invalid index buffer handle!");

        const THashMap<uint32_t, TArray<FMeshPart>>& perMaterialMeshParts = staticMesh->getPerMaterialMeshParts();

        for (const auto& materialSlots : staticMesh->getMaterialSlots())
        {
            const TArray<FMeshPart>& meshParts = perMaterialMeshParts[materialSlots.key];

            for (const FMeshPart& meshPart : meshParts)
            {
                bindVertexBuffer(vertexBuffer, meshPart.vertexOffset);
                bindIndexBuffer(indexBuffer, meshPart.indexOffset, EIndexFormat::Uint32);
                drawIndexed(FDrawIndexedCommand(meshPart.indexSize / sizeof(uint32_t), 1, 0, 0, 0));
            }
        }
    }

    void FCommandBufferImpl::dispatch(const uint32_t groupCountX, const uint32_t groupCountY, const uint32_t groupCountZ)
    {
        vkCmdDispatch(m_Handle, groupCountX, groupCountY, groupCountZ);
    }

    void FCommandBufferImpl::dispatchIndirect(IBuffer* buffer, const int64_t offset)
    {
        LUMA_CHECK(buffer, "Invalid buffer handle!");
        const FBufferImpl* bufferImpl = static_cast<const FBufferImpl*>(buffer);
        vkCmdDispatchIndirect(m_Handle, bufferImpl->getHandle(), offset);
    }

    void FCommandBufferImpl::copyBuffer(IBuffer* srcBuffer, IBuffer* dstBuffer, const int64_t srcOffset, const int64_t dstOffset, const uint64_t size)
    {
        LUMA_CHECK(srcBuffer, "Invalid buffer handle!");
        LUMA_CHECK(dstBuffer, "Invalid buffer handle!");

        VkBufferCopy2 region = { VK_STRUCTURE_TYPE_BUFFER_COPY_2 };
        region.srcOffset = srcOffset;
        region.dstOffset = dstOffset;
        region.size = size;

        const FBufferImpl* srcBufferImpl = static_cast<const FBufferImpl*>(srcBuffer);
        const FBufferImpl* dstBufferImpl = static_cast<const FBufferImpl*>(dstBuffer);
        VkCopyBufferInfo2 copyInfo { VK_STRUCTURE_TYPE_COPY_BUFFER_INFO_2 };
        copyInfo.srcBuffer = srcBufferImpl->getHandle();
        copyInfo.dstBuffer = dstBufferImpl->getHandle();
        copyInfo.regionCount = 1;
        copyInfo.pRegions = &region;
        vkCmdCopyBuffer2(m_Handle, &copyInfo);
    }

    void FCommandBufferImpl::copyBufferToTexture(IBuffer* buffer, const int64_t offset, uint64_t size, ITexture* texture, const uint32_t arrayIndex, const uint32_t mipLevel)
    {
        LUMA_CHECK(buffer, "Invalid buffer handle!");
        LUMA_CHECK(texture, "Invalid texture handle!");

        const FBufferImpl* bufferImpl = static_cast<const FBufferImpl*>(buffer);
        const FTextureImpl* textureImpl = static_cast<const FTextureImpl*>(texture);

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

    void FCommandBufferImpl::textureBarrier(const FTextureBarrier& barrier)
    {
        FTextureImpl* texture = static_cast<FTextureImpl*>(barrier.texture);
        LUMA_CHECK(texture, "Invalid texture handle!");

        const VkImageMemoryBarrier vkBarrier = makeTextureBarrier(barrier);
        const VkPipelineStageFlags srcStageFlags = getSourcePipelineStageFlags(barrier.sourceAccess);
        const VkPipelineStageFlags dstStageFlags = getDestPipelineStageFlags(barrier.destAccess);
        vkCmdPipelineBarrier(m_Handle, srcStageFlags, dstStageFlags, 0, 0, nullptr, 0, nullptr, 1, &vkBarrier);
        texture->setResourceState(barrier.destState);
    }

    void FCommandBufferImpl::bufferBarrier(const FBufferBarrier& barrier)
    {
        FBufferImpl* buffer = static_cast<FBufferImpl*>(barrier.buffer);
        LUMA_CHECK(buffer, "Invalid buffer handle!");

        const VkBufferMemoryBarrier vkBarrier = makeBufferBarrier(barrier);
        const VkPipelineStageFlags srcStageFlags = getSourcePipelineStageFlags(barrier.sourceAccess);
        const VkPipelineStageFlags dstStageFlags = getSourcePipelineStageFlags(barrier.destAccess);
        vkCmdPipelineBarrier(m_Handle, srcStageFlags, dstStageFlags, 0, 0, nullptr, 1, &vkBarrier, 0, nullptr);
        buffer->setResourceState(barrier.destState);
    }

    void FCommandBufferImpl::setName(const FStringView name)
    {
        setVulkanObjectDebugName(m_Device, VK_OBJECT_TYPE_COMMAND_BUFFER, m_Handle, name);
    }
}
