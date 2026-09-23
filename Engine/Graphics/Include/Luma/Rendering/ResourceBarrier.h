#pragma once
#include "ResourceState.h"
#include "Luma/Runtime/Flags.h"

namespace Luma
{
    namespace RHI
    {
        struct Texture;
        struct Buffer;
        struct Queue;
    }

    enum class ResourceAccess
    {
        None = 0,
        ShaderRead = BIT(0),
        ShaderWrite = BIT(1),
        ColorTargetRead = BIT(2),
        ColorTargetWrite = BIT(3),
        DepthStencilTargetRead = BIT(4),
        DepthStencilTargetWrite = BIT(5),
        CopyRead = BIT(6),
        CopyWrite = BIT(7),
        HostRead = BIT(8),
        HostWrite = BIT(9),
    };

    using ResourceAccessFlags = TFlags<ResourceAccess>;

    struct TextureBarrier
    {
        RHI::Texture* texture = nullptr;
        ResourceState destState = ResourceState::Undefined;
        ResourceAccessFlags sourceAccess = ResourceAccess::None;
        ResourceAccessFlags destAccess = ResourceAccess::None;
        const RHI::Queue* sourceQueue = nullptr;
        const RHI::Queue* destQueue = nullptr;
    };

    struct BufferBarrier
    {
        RHI::Buffer* buffer = nullptr;
        uint64_t offset = 0;
        uint64_t size = 0;
        ResourceState destState = ResourceState::Undefined;
        ResourceAccessFlags sourceAccess = ResourceAccess::None;
        ResourceAccessFlags destAccess = ResourceAccess::None;
        const RHI::Queue* sourceQueue = nullptr;
        const RHI::Queue* destQueue = nullptr;
    };

    ResourceAccessFlags getSourceAccessFlags(ResourceState resourceState);
    ResourceAccessFlags getDestAccessFlags(ResourceState resourceState);
}
