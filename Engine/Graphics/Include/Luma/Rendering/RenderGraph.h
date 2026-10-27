#pragma once
#include "Forward.h"
#include "LoadOperation.h"
#include "RenderPassDesc.h"
#include "ResourceState.h"
#include "ResourceType.h"
#include "TextureUsage.h"
#include "Luma/Containers/Array.h"
#include "Luma/Containers/String.h"
#include "Luma/Graphics/Export.h"
#include "Luma/Math/Color.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Memory/RefCounted.h"
#include "Luma/Runtime/Format.h"

#include <functional>
#include <memory>

namespace Luma
{
    struct RenderGraphTextureDesc
    {
        Format format = Format::None;
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t depth = 1;
        uint32_t mipCount = 1;
        uint32_t sampleCount = 1;
        uint32_t arrayCount = 1;

        bool operator==(const RenderGraphTextureDesc&) const = default;
    };

    /// Frame graph that orders passes, culls unused ones, allocates transient textures
    /// and records the resource barriers between passes.
    ///
    /// The graph is rebuilt every frame:
    /// @code
    /// graph.reset();
    /// RenderGraph::TextureHandle backbuffer = graph.importTexture("Backbuffer", device->getAcquiredSwapchainTexture());
    ///
    /// struct SceneData { RenderGraph::TextureHandle color; };
    /// const SceneData& scene = graph.addPass<SceneData>("Scene",
    ///     [&](RenderGraph::PassBuilder& builder, SceneData& data)
    ///     {
    ///         data.color = builder.createTexture("SceneColor", {Format::R8G8B8A8_SRGB, width, height});
    ///         builder.addColorTarget(data.color);
    ///     },
    ///     [=](const SceneData& data, const RenderGraph::PassContext& context)
    ///     {
    ///         context.getCommandBuffer()->draw(...);
    ///     });
    ///
    /// graph.addPass("Blit",
    ///     [&](RenderGraph::PassBuilder& builder)
    ///     {
    ///         builder.readTexture(scene.color);
    ///         builder.addColorTarget(backbuffer);
    ///     },
    ///     [&, color = scene.color](const RenderGraph::PassContext& context)
    ///     {
    ///         bindingGroup->bindTextureWithSampler(0, context.getTexture(color), sampler);
    ///         ...
    ///     });
    ///
    /// graph.execute(device->getCommandBuffer());
    /// @endcode
    class LUMA_GRAPHICS_API RenderGraph
    {
    public:
        static constexpr uint32_t kInvalidIndex = ~0u;

        template<typename T>
        struct Handle
        {
            uint32_t index = kInvalidIndex;

            bool isValid() const { return index != kInvalidIndex; }
            bool operator==(const Handle&) const = default;
        };

        using TextureHandle = Handle<RHI::Texture>;
        using BufferHandle = Handle<RHI::Buffer>;

        /// Declares the resources a pass uses. Only valid inside a pass setup function.
        class LUMA_GRAPHICS_API PassBuilder
        {
        public:
            /// Declares a texture owned by the graph. Its memory may be shared with other
            /// transient textures whose lifetimes do not overlap.
            TextureHandle createTexture(String name, const RenderGraphTextureDesc& desc);

            TextureHandle readTexture(TextureHandle texture, ResourceState state = ResourceState::ShaderRead);
            TextureHandle writeTexture(TextureHandle texture, ResourceState state = ResourceState::ShaderWrite);
            BufferHandle readBuffer(BufferHandle buffer, ResourceState state = ResourceState::ShaderRead);
            BufferHandle writeBuffer(BufferHandle buffer, ResourceState state = ResourceState::ShaderWrite);

            /// Binds a color target. Targets are bound in the order they are added.
            /// The render pass, viewport and scissor are set up by the graph before the pass executes.
            TextureHandle addColorTarget(TextureHandle texture, LoadOp loadOp = LoadOp::Clear, const Color& clearColor = Color::Black);
            TextureHandle setDepthStencilTarget(TextureHandle texture, LoadOp loadOp = LoadOp::Clear, float clearDepth = 1.0f, uint32_t clearStencil = 0);

            /// Keeps the pass even if nothing reads what it writes
            void setNeverCull();

        private:
            friend class RenderGraph;
            PassBuilder(RenderGraph& graph, uint32_t passIndex) : m_Graph(graph), m_PassIndex(passIndex) {}

            void use(uint32_t resourceIndex, ResourceState state, bool read, bool write);

            RenderGraph& m_Graph;
            uint32_t m_PassIndex;
        };

        /// Gives a pass access to its command buffer and to the physical resources behind handles
        class LUMA_GRAPHICS_API PassContext
        {
        public:
            RHI::CommandBuffer* getCommandBuffer() const { return m_CommandBuffer; }
            RHI::Texture* getTexture(TextureHandle handle) const;
            RHI::Buffer* getBuffer(BufferHandle handle) const;

        private:
            friend class RenderGraph;
            PassContext(const RenderGraph& graph, RHI::CommandBuffer* cmdBuffer) : m_Graph(graph), m_CommandBuffer(cmdBuffer) {}

            const RenderGraph& m_Graph;
            RHI::CommandBuffer* m_CommandBuffer;
        };

        explicit RenderGraph(Ref<RHI::Device> device);
        ~RenderGraph();
        LUMA_NO_COPYABLE(RenderGraph);

        /// Registers a texture owned outside of the graph (e.g. the swapchain texture).
        /// Passes writing to imported resources are never culled.
        TextureHandle importTexture(String name, RHI::Texture* texture);
        BufferHandle importBuffer(String name, RHI::Buffer* buffer);

        /// Adds a pass whose setup function fills a Data struct read back by the execute function.
        /// @param setup void(PassBuilder&, Data&), called immediately
        /// @param exec void(const Data&, const PassContext&), called from execute() unless the pass is culled
        /// @return The pass data, valid until reset(). Use it to forward handles to later passes.
        template<typename Data, typename SetupFn, typename ExecFn>
        const Data& addPass(String name, SetupFn&& setup, ExecFn&& exec)
        {
            std::shared_ptr<Data> data = std::make_shared<Data>();
            const uint32_t passIndex = createPass(std::move(name));
            PassBuilder builder(*this, passIndex);
            setup(builder, *data);

            m_Passes[passIndex].execute = [data, exec = std::forward<ExecFn>(exec)](const PassContext& context)
            {
                exec(*data, context);
            };
            return *data;
        }

        /// Adds a pass without pass data
        /// @param setup void(PassBuilder&), called immediately
        /// @param exec void(const PassContext&), called from execute() unless the pass is culled
        template<typename SetupFn, typename ExecFn>
        void addPass(String name, SetupFn&& setup, ExecFn&& exec)
        {
            struct NoData {};
            addPass<NoData>(std::move(name),
                [&setup](PassBuilder& builder, NoData&) { setup(builder); },
                [exec = std::forward<ExecFn>(exec)](const NoData&, const PassContext& context) { exec(context); });
        }

        /// Culls unused passes, computes resource lifetimes and allocates transient textures.
        /// Called by execute() if needed.
        void compile();

        /// Records every non-culled pass into the command buffer, in the order they were added
        void execute(RHI::CommandBuffer* cmdBuffer);

        /// Clears passes and resources to build the next frame. Transient textures are kept for reuse.
        void reset();

    private:
        struct ResourceUsage
        {
            uint32_t resourceIndex = kInvalidIndex;
            ResourceState state = ResourceState::Undefined;
            bool read = false;
            bool write = false;
        };

        struct RenderTarget
        {
            uint32_t resourceIndex = kInvalidIndex;
            LoadOp loadOp = LoadOp::Clear;
            RHI::ClearValue clearValue;
        };

        struct Pass
        {
            String name;
            Array<ResourceUsage> usages;
            Array<RenderTarget> colorTargets;
            RenderTarget depthStencilTarget;
            std::function<void(const PassContext&)> execute;
            bool neverCull = false;
            bool culled = false;
        };

        struct ResourceNode
        {
            String name;
            ResourceType type = ResourceType::Texture;
            RenderGraphTextureDesc textureDesc;
            TextureUsageFlags textureUsage = TextureUsage::None;
            RHI::Texture* texture = nullptr;
            RHI::Buffer* buffer = nullptr;
            uint32_t firstPass = kInvalidIndex;
            uint32_t lastPass = kInvalidIndex;
            uint32_t poolIndex = kInvalidIndex;
            bool imported = false;
        };

        struct PooledTexture
        {
            Ref<RHI::Texture> texture = nullptr;
            RenderGraphTextureDesc desc;
            TextureUsageFlags usage = TextureUsage::None;
            uint64_t lastUsedFrame = 0;
            bool inUse = false;
        };

        uint32_t createPass(String name);
        uint32_t createResource(String name, ResourceType type);
        const ResourceNode& getResource(uint32_t index, ResourceType type) const;
        void cullPasses();
        void computeLifetimes();
        void allocateTransientTextures();
        uint32_t acquirePooledTexture(const RenderGraphTextureDesc& desc, TextureUsageFlags usage);
        void recordBarriers(const Pass& pass, RHI::CommandBuffer* cmdBuffer) const;
        void executePass(const Pass& pass, RHI::CommandBuffer* cmdBuffer) const;

        Ref<RHI::Device> m_Device = nullptr;
        Array<Pass> m_Passes;
        Array<ResourceNode> m_Resources;
        Array<PooledTexture> m_TexturePool;
        uint64_t m_FrameIndex = 0;
        bool m_Compiled = false;
    };
}
