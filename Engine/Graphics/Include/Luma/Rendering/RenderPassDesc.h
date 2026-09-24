#pragma once
#include "ResolveMode.h"
#include "LoadOperation.h"
#include "StoreOperation.h"
#include "Luma/Math/Color.h"
#include "Luma/Containers/Array.h"
#include "Luma/Math/Rect2.h"

namespace Luma::RHI
{
    struct TextureView;

    struct ClearValue
    {
        Color color{Color::Black};
        float depth = 1.0f;
        uint32_t stencil = 0;
    };

    enum class RenderPassTargetType
    {
        Color,
        DepthStencil,
    };

    struct RenderPassTarget
    {
        RenderPassTargetType type = RenderPassTargetType::Color;
        ClearValue clearValue = ClearValue();
        LoadOp loadOp = LoadOp::Clear;
        StoreOp storeOp = StoreOp::Store;
        ResolveMode resolveMode = ResolveMode::None;
        const TextureView* textureView = nullptr;
        const TextureView* resolveTextureView = nullptr;
    };

    struct RenderPassDesc
    {
        Array<RenderPassTarget*> colorTargets;
        RenderPassTarget* depthStencilTarget = nullptr;
        FRect2u renderArea;
    };
}
