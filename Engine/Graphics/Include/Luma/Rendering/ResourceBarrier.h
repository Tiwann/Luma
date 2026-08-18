#pragma once
#include "ResourceState.h"
#include "Luma/Runtime/Flags.h"

namespace Luma
{
    struct ITexture;
    struct IBuffer;
    struct IQueue;

    enum class EResourceAccessBits
    {
        None = 0,
        ShaderRead = BIT(0),
        ShaderWrite = BIT(1),
        ColorAttachmentRead = BIT(2),
        ColorAttachmentWrite = BIT(3),
        DepthStencilAttachmentRead = BIT(4),
        DepthStencilAttachmentWrite = BIT(5),
        TransferRead = BIT(6),
        TransferWrite = BIT(7),
        HostRead = BIT(8),
        HostWrite = BIT(9),
    };

    using FResourceAccessFlags = TFlags<EResourceAccessBits>;

    struct FTextureBarrier
    {
        ITexture* texture = nullptr;
        EResourceState destState = EResourceState::Undefined;
        FResourceAccessFlags sourceAccess = EResourceAccessBits::None;
        FResourceAccessFlags destAccess = EResourceAccessBits::None;
        const IQueue* sourceQueue = nullptr;
        const IQueue* destQueue = nullptr;
    };

    struct FBufferBarrier
    {
        IBuffer* buffer = nullptr;
        uint64_t offset = 0;
        uint64_t size = 0;
        EResourceState destState = EResourceState::Undefined;
        FResourceAccessFlags sourceAccess = EResourceAccessBits::None;
        FResourceAccessFlags destAccess = EResourceAccessBits::None;
        const IQueue* sourceQueue = nullptr;
        const IQueue* destQueue = nullptr;
    };

    FResourceAccessFlags getSourceAccessFlags(EResourceState resourceState);
    FResourceAccessFlags getDestAccessFlags(EResourceState resourceState);
}
