#include "Luma/Rendering/RenderGraph.h"
#include "Luma/Rendering/Buffer.h"
#include "Luma/Rendering/CommandBuffer.h"
#include "Luma/Rendering/Constants.h"
#include "Luma/Rendering/Device.h"
#include "Luma/Rendering/ResourceBarrier.h"
#include "Luma/Rendering/Scissor.h"
#include "Luma/Rendering/Texture.h"
#include "Luma/Rendering/Viewport.h"

namespace Luma
{
    // Pooled textures unused for this many frames are destroyed. Must stay above the number
    // of frames in flight so the GPU is done with a texture before it gets destroyed.
    static constexpr uint64_t kMaxUnusedFrames = 8;
    static_assert(kMaxUnusedFrames > RHI::NUM_FRAMES_IN_FLIGHT);

    static bool hasWriteAccess(const ResourceAccessFlags access)
    {
        return access.contains(ResourceAccess::ShaderWrite)
            || access.contains(ResourceAccess::ColorTargetWrite)
            || access.contains(ResourceAccess::DepthStencilTargetWrite)
            || access.contains(ResourceAccess::CopyWrite)
            || access.contains(ResourceAccess::HostWrite);
    }

    static ResourceAccessFlags getCurrentAccess(const ResourceState state)
    {
        // General carries no access information, assume any shader access happened
        if (state == ResourceState::General)
            return ResourceAccess::ShaderRead | ResourceAccess::ShaderWrite;
        return getSourceAccessFlags(state);
    }

    static ResourceAccessFlags getUsageAccess(const ResourceState state, const bool read, const bool write)
    {
        ResourceAccessFlags access = ResourceAccess::None;
        switch (state)
        {
        case ResourceState::General:
        case ResourceState::ShaderRead:
        case ResourceState::ShaderWrite:
            if (read) access |= ResourceAccess::ShaderRead;
            if (write) access |= ResourceAccess::ShaderWrite;
            break;
        case ResourceState::ColorTarget:
            if (read) access |= ResourceAccess::ColorTargetRead;
            if (write) access |= ResourceAccess::ColorTargetWrite;
            break;
        case ResourceState::DepthStencilTarget:
            if (read) access |= ResourceAccess::DepthStencilTargetRead;
            if (write) access |= ResourceAccess::DepthStencilTargetWrite;
            break;
        case ResourceState::CopySource:
        case ResourceState::CopyDest:
            if (read) access |= ResourceAccess::CopyRead;
            if (write) access |= ResourceAccess::CopyWrite;
            break;
        default:
            break;
        }
        return access;
    }

    static TextureUsage getTextureUsage(const ResourceState state)
    {
        switch (state)
        {
        case ResourceState::ShaderRead: return TextureUsage::Sampled;
        case ResourceState::General:
        case ResourceState::ShaderWrite: return TextureUsage::Storage;
        case ResourceState::ColorTarget:
        case ResourceState::DepthStencilTarget: return TextureUsage::RenderTarget;
        default: return TextureUsage::None;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// PASS BUILDER
    ///////////////////////////////////////////////////////////////////////////////////////////////
    RenderGraph::TextureHandle RenderGraph::PassBuilder::createTexture(String name, const RenderGraphTextureDesc& desc)
    {
        LUMA_ASSERT(desc.format != Format::None && desc.width > 0 && desc.height > 0, "Invalid render graph texture description");
        const uint32_t resourceIndex = m_Graph.createResource(std::move(name), ResourceType::Texture);
        m_Graph.m_Resources[resourceIndex].textureDesc = desc;
        return TextureHandle{resourceIndex};
    }

    RenderGraph::TextureHandle RenderGraph::PassBuilder::readTexture(const TextureHandle texture, const ResourceState state)
    {
        use(texture.index, state, true, false);
        return texture;
    }

    RenderGraph::TextureHandle RenderGraph::PassBuilder::writeTexture(const TextureHandle texture, const ResourceState state)
    {
        use(texture.index, state, false, true);
        return texture;
    }

    RenderGraph::BufferHandle RenderGraph::PassBuilder::readBuffer(const BufferHandle buffer, const ResourceState state)
    {
        use(buffer.index, state, true, false);
        return buffer;
    }

    RenderGraph::BufferHandle RenderGraph::PassBuilder::writeBuffer(const BufferHandle buffer, const ResourceState state)
    {
        use(buffer.index, state, false, true);
        return buffer;
    }

    RenderGraph::TextureHandle RenderGraph::PassBuilder::addColorTarget(const TextureHandle texture, const LoadOp loadOp, const Color& clearColor)
    {
        RenderTarget target;
        target.resourceIndex = texture.index;
        target.loadOp = loadOp;
        target.clearValue.color = clearColor;
        m_Graph.m_Passes[m_PassIndex].colorTargets.add(target);

        use(texture.index, ResourceState::ColorTarget, loadOp == LoadOp::Load, true);
        return texture;
    }

    RenderGraph::TextureHandle RenderGraph::PassBuilder::setDepthStencilTarget(const TextureHandle texture, const LoadOp loadOp, const float clearDepth, const uint32_t clearStencil)
    {
        Pass& pass = m_Graph.m_Passes[m_PassIndex];
        LUMA_ASSERT(pass.depthStencilTarget.resourceIndex == kInvalidIndex, "Pass already has a depth stencil target");

        pass.depthStencilTarget.resourceIndex = texture.index;
        pass.depthStencilTarget.loadOp = loadOp;
        pass.depthStencilTarget.clearValue.depth = clearDepth;
        pass.depthStencilTarget.clearValue.stencil = clearStencil;

        use(texture.index, ResourceState::DepthStencilTarget, loadOp == LoadOp::Load, true);
        return texture;
    }

    void RenderGraph::PassBuilder::setNeverCull()
    {
        m_Graph.m_Passes[m_PassIndex].neverCull = true;
    }

    void RenderGraph::PassBuilder::use(const uint32_t resourceIndex, const ResourceState state, const bool read, const bool write)
    {
        LUMA_ASSERT(resourceIndex < m_Graph.m_Resources.count(), "Invalid render graph resource handle");
        m_Graph.m_Passes[m_PassIndex].usages.add({resourceIndex, state, read, write});
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// PASS CONTEXT
    ///////////////////////////////////////////////////////////////////////////////////////////////
    RHI::Texture* RenderGraph::PassContext::getTexture(const TextureHandle handle) const
    {
        return m_Graph.getResource(handle.index, ResourceType::Texture).texture;
    }

    RHI::Buffer* RenderGraph::PassContext::getBuffer(const BufferHandle handle) const
    {
        return m_Graph.getResource(handle.index, ResourceType::Buffer).buffer;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// RENDER GRAPH
    ///////////////////////////////////////////////////////////////////////////////////////////////
    RenderGraph::RenderGraph(Ref<RHI::Device> device) : m_Device(std::move(device))
    {
    }

    RenderGraph::~RenderGraph() = default;

    RenderGraph::TextureHandle RenderGraph::importTexture(String name, RHI::Texture* texture)
    {
        LUMA_ASSERT(texture, "Cannot import a null texture");
        const uint32_t resourceIndex = createResource(std::move(name), ResourceType::Texture);
        ResourceNode& resource = m_Resources[resourceIndex];
        resource.texture = texture;
        resource.imported = true;
        return TextureHandle{resourceIndex};
    }

    RenderGraph::BufferHandle RenderGraph::importBuffer(String name, RHI::Buffer* buffer)
    {
        LUMA_ASSERT(buffer, "Cannot import a null buffer");
        const uint32_t resourceIndex = createResource(std::move(name), ResourceType::Buffer);
        ResourceNode& resource = m_Resources[resourceIndex];
        resource.buffer = buffer;
        resource.imported = true;
        return BufferHandle{resourceIndex};
    }

    void RenderGraph::compile()
    {
        cullPasses();
        computeLifetimes();
        allocateTransientTextures();
        m_Compiled = true;
    }

    void RenderGraph::execute(RHI::CommandBuffer* cmdBuffer)
    {
        LUMA_ASSERT(cmdBuffer, "Cannot execute a render graph without a command buffer");
        if (!m_Compiled)
            compile();

        for (const Pass& pass : m_Passes)
        {
            if (pass.culled)
                continue;

            cmdBuffer->beginDebugGroup(pass.name, Color::Orange);
            recordBarriers(pass, cmdBuffer);
            executePass(pass, cmdBuffer);
            cmdBuffer->endDebugGroup();
        }
    }

    void RenderGraph::reset()
    {
        // free() rather than clear() so pass closures release what they captured
        m_Passes.free();
        m_Resources.free();
        m_Compiled = false;
        ++m_FrameIndex;

        bool hasExpiredTextures = false;
        for (const PooledTexture& pooled : m_TexturePool)
            hasExpiredTextures |= m_FrameIndex - pooled.lastUsedFrame > kMaxUnusedFrames;

        if (!hasExpiredTextures)
            return;

        Array<PooledTexture> pool;
        for (const PooledTexture& pooled : m_TexturePool)
        {
            if (m_FrameIndex - pooled.lastUsedFrame <= kMaxUnusedFrames)
                pool.add(pooled);
        }
        m_TexturePool = std::move(pool);
    }

    uint32_t RenderGraph::createPass(String name)
    {
        Pass pass;
        pass.name = std::move(name);
        m_Passes.emplace(std::move(pass));
        m_Compiled = false;
        return static_cast<uint32_t>(m_Passes.count() - 1);
    }

    uint32_t RenderGraph::createResource(String name, const ResourceType type)
    {
        ResourceNode resource;
        resource.name = std::move(name);
        resource.type = type;
        m_Resources.emplace(std::move(resource));
        m_Compiled = false;
        return static_cast<uint32_t>(m_Resources.count() - 1);
    }

    const RenderGraph::ResourceNode& RenderGraph::getResource(const uint32_t index, const ResourceType type) const
    {
        LUMA_ASSERT(index < m_Resources.count(), "Invalid render graph resource handle");
        const ResourceNode& resource = m_Resources[index];
        LUMA_ASSERT(resource.type == type, "Render graph resource type mismatch");
        return resource;
    }

    void RenderGraph::cullPasses()
    {
        // Walk passes backward: a pass is needed if it never culls, or writes a resource that is
        // imported or read by a later needed pass. The resources a needed pass reads become needed.
        Array<bool> neededResources(m_Resources.count());

        for (uint32_t passIndex = static_cast<uint32_t>(m_Passes.count()); passIndex-- > 0;)
        {
            Pass& pass = m_Passes[passIndex];
            bool needed = pass.neverCull;
            for (const ResourceUsage& usage : pass.usages)
            {
                if (usage.write && (m_Resources[usage.resourceIndex].imported || neededResources[usage.resourceIndex]))
                    needed = true;
            }

            pass.culled = !needed;
            if (!needed)
                continue;

            for (const ResourceUsage& usage : pass.usages)
            {
                if (usage.read)
                    neededResources[usage.resourceIndex] = true;
            }
        }
    }

    void RenderGraph::computeLifetimes()
    {
        for (ResourceNode& resource : m_Resources)
        {
            resource.firstPass = kInvalidIndex;
            resource.lastPass = kInvalidIndex;
            if (!resource.imported)
                resource.textureUsage = isDepthFormat(resource.textureDesc.format) ? TextureUsage::DepthStencil : TextureUsage::Color;
        }

        for (uint32_t passIndex = 0; passIndex < m_Passes.count(); ++passIndex)
        {
            const Pass& pass = m_Passes[passIndex];
            if (pass.culled)
                continue;

            for (const ResourceUsage& usage : pass.usages)
            {
                ResourceNode& resource = m_Resources[usage.resourceIndex];
                if (resource.firstPass == kInvalidIndex)
                {
                    LUMA_ASSERT(resource.imported || !usage.read, "Transient texture is read before being written");
                    resource.firstPass = passIndex;
                }
                resource.lastPass = passIndex;
                resource.textureUsage |= getTextureUsage(usage.state);
            }
        }
    }

    void RenderGraph::allocateTransientTextures()
    {
        for (PooledTexture& pooled : m_TexturePool)
            pooled.inUse = false;

        for (ResourceNode& resource : m_Resources)
        {
            if (resource.imported)
                continue;
            resource.texture = nullptr;
            resource.poolIndex = kInvalidIndex;
        }

        // Textures go back to the pool after their last pass, so transient textures
        // first used later in the frame can reuse them.
        for (uint32_t passIndex = 0; passIndex < m_Passes.count(); ++passIndex)
        {
            const Pass& pass = m_Passes[passIndex];
            if (pass.culled)
                continue;

            for (const ResourceUsage& usage : pass.usages)
            {
                ResourceNode& resource = m_Resources[usage.resourceIndex];
                if (resource.imported || resource.firstPass != passIndex || resource.poolIndex != kInvalidIndex)
                    continue;

                resource.poolIndex = acquirePooledTexture(resource.textureDesc, resource.textureUsage);
                resource.texture = m_TexturePool[resource.poolIndex].texture;
                resource.texture->setName(resource.name);
            }

            for (const ResourceUsage& usage : pass.usages)
            {
                const ResourceNode& resource = m_Resources[usage.resourceIndex];
                if (!resource.imported && resource.lastPass == passIndex)
                    m_TexturePool[resource.poolIndex].inUse = false;
            }
        }
    }

    uint32_t RenderGraph::acquirePooledTexture(const RenderGraphTextureDesc& desc, const TextureUsageFlags usage)
    {
        for (uint32_t index = 0; index < m_TexturePool.count(); ++index)
        {
            PooledTexture& pooled = m_TexturePool[index];
            if (!pooled.inUse && pooled.desc == desc && pooled.usage == usage)
            {
                pooled.inUse = true;
                pooled.lastUsedFrame = m_FrameIndex;
                return index;
            }
        }

        RHI::TextureDesc textureDesc;
        textureDesc.usageFlags = usage;
        textureDesc.format = desc.format;
        textureDesc.width = desc.width;
        textureDesc.height = desc.height;
        textureDesc.depth = desc.depth;
        textureDesc.mipCount = desc.mipCount;
        textureDesc.sampleCount = desc.sampleCount;
        textureDesc.arrayCount = desc.arrayCount;

        PooledTexture pooled;
        pooled.texture = m_Device->createTexture(textureDesc);
        LUMA_ASSERT(pooled.texture, "Failed to create render graph texture");
        pooled.desc = desc;
        pooled.usage = usage;
        pooled.lastUsedFrame = m_FrameIndex;
        pooled.inUse = true;
        m_TexturePool.add(pooled);
        return static_cast<uint32_t>(m_TexturePool.count() - 1);
    }

    void RenderGraph::recordBarriers(const Pass& pass, RHI::CommandBuffer* cmdBuffer) const
    {
        Array<TextureBarrier> textureBarriers;
        Array<BufferBarrier> bufferBarriers;

        for (const ResourceUsage& usage : pass.usages)
        {
            const ResourceNode& resource = m_Resources[usage.resourceIndex];
            const bool isTexture = resource.type == ResourceType::Texture;

            // Resources track their own state, which is also correct for textures aliased from the pool
            const ResourceState currentState = isTexture ? resource.texture->getResourceState() : resource.buffer->getResourceState();
            const ResourceAccessFlags sourceAccess = getCurrentAccess(currentState);
            const ResourceAccessFlags destAccess = getUsageAccess(usage.state, usage.read, usage.write);

            // Reads in the same state don't need synchronization
            if (currentState == usage.state && !hasWriteAccess(sourceAccess) && !hasWriteAccess(destAccess))
                continue;

            if (isTexture)
            {
                TextureBarrier barrier;
                barrier.texture = resource.texture;
                barrier.destState = usage.state;
                barrier.sourceAccess = sourceAccess;
                barrier.destAccess = destAccess;
                textureBarriers.add(barrier);
            }
            else
            {
                BufferBarrier barrier;
                barrier.buffer = resource.buffer;
                barrier.offset = 0;
                barrier.size = resource.buffer->getSize();
                barrier.destState = usage.state;
                barrier.sourceAccess = sourceAccess;
                barrier.destAccess = destAccess;
                bufferBarriers.add(barrier);
            }
        }

        if (!textureBarriers.isEmpty())
            cmdBuffer->textureBarriers(textureBarriers);
        if (!bufferBarriers.isEmpty())
            cmdBuffer->bufferBarriers(bufferBarriers);
    }

    void RenderGraph::executePass(const Pass& pass, RHI::CommandBuffer* cmdBuffer) const
    {
        const PassContext context(*this, cmdBuffer);
        const bool hasDepthStencilTarget = pass.depthStencilTarget.resourceIndex != kInvalidIndex;
        if (pass.colorTargets.isEmpty() && !hasDepthStencilTarget)
        {
            pass.execute(context);
            return;
        }

        const auto makeTarget = [this](const RenderTarget& target, const RHI::RenderPassTargetType type)
        {
            RHI::RenderPassTarget result;
            result.type = type;
            result.clearValue = target.clearValue;
            result.loadOp = target.loadOp;
            result.storeOp = StoreOp::Store;
            result.textureView = m_Resources[target.resourceIndex].texture->getTextureView();
            return result;
        };

        // RenderPassDesc points to its targets, so fill them all before taking their addresses
        Array<RHI::RenderPassTarget> colorTargets(pass.colorTargets.count());
        for (uint32_t index = 0; index < pass.colorTargets.count(); ++index)
            colorTargets[index] = makeTarget(pass.colorTargets[index], RHI::RenderPassTargetType::Color);

        RHI::RenderPassDesc renderPassDesc;
        for (RHI::RenderPassTarget& target : colorTargets)
            renderPassDesc.colorTargets.add(&target);

        RHI::RenderPassTarget depthStencilTarget;
        if (hasDepthStencilTarget)
        {
            depthStencilTarget = makeTarget(pass.depthStencilTarget, RHI::RenderPassTargetType::DepthStencil);
            renderPassDesc.depthStencilTarget = &depthStencilTarget;
        }

        const uint32_t areaResourceIndex = pass.colorTargets.isEmpty() ? pass.depthStencilTarget.resourceIndex : pass.colorTargets.first().resourceIndex;
        const RHI::Texture* areaTexture = m_Resources[areaResourceIndex].texture;
        const uint32_t width = areaTexture->getWidth();
        const uint32_t height = areaTexture->getHeight();
        renderPassDesc.renderArea = FRect2u(0, 0, width, height);

        cmdBuffer->beginRenderPass(renderPassDesc);
        cmdBuffer->setViewport(Viewport::fromSize(static_cast<float>(width), static_cast<float>(height)));
        cmdBuffer->setScissor(Scissor::fromSize(width, height));
        pass.execute(context);
        cmdBuffer->endRenderPass();
    }
}
