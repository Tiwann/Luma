#pragma once
#include "ResolveMode.h"
#include "LoadOperation.h"
#include "StoreOperation.h"
#include "Luma/Math/Color.h"
#include "Luma/Containers/Array.h"
#include "Luma/Math/Rect2.h"

namespace Luma
{
    struct ITextureView;

    struct FClearValue
    {
        FColor color;
        float depth = 1.0f;
        uint32_t stencil = 0;
    };

    enum class ERenderPassAttachmentType
    {
        Color,
        DepthStencil,
    };

    struct FRenderPassAttachment
    {
        ERenderPassAttachmentType type = ERenderPassAttachmentType::Color;
        FClearValue clearValue = FClearValue();
        ELoadOp loadOp = ELoadOp::Load;
        EStoreOp storeOp = EStoreOp::Store;
        EResolveMode resolveMode = EResolveMode::None;
        const ITextureView* textureView = nullptr;
        const ITextureView* resolveTextureView = nullptr;
    };

    struct FRenderPassDesc
    {
        TArray<FRenderPassAttachment*> colorAttachments;
        FRenderPassAttachment* depthStencilAttachment = nullptr;
        FRect2u renderArea;
    };
}
