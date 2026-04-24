#pragma once
#include "Object.h"

namespace Luma
{
    enum class EAssetType
    {
        Texture,
        AudioClip,
        AudioStream,
        Shader,
        StaticMesh,
        SkinnedMesh,
    };

    struct IAsset : IObject
    {
        ~IAsset() override = default;
        virtual EAssetType getAssetType() const = 0;
    };
}