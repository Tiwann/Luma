#pragma once
#include "ResolveMode.h"
#include "LoadOperation.h"
#include "StoreOperation.h"
#include "Luma/Math/Color.h"

namespace Luma
{
    struct ITextureView;

    struct ClearValue
    {
        FColor color;
        float depth = 1.0f;
        uint32_t stencil = 0;
    };

    struct FrameBufferAttachment
    {
        ClearValue clearValue = ClearValue();
        ELoadOp loadOp = ELoadOp::Load;
        EStoreOp storeOp = EStoreOp::Store;
        EResolveMode resolveMode = EResolveMode::None;
        const ITextureView* textureView = nullptr;
        const ITextureView* resolveTextureView = nullptr;
    };

    struct FrameBufferInfo
    {
        FrameBufferAttachment colorAttachments[8];
        uint32_t colorAttachmentCount;
        FrameBufferAttachment depthAttachment;
    };
}
