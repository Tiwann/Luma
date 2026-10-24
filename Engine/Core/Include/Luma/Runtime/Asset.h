#pragma once
#include "Object.h"

namespace Luma
{
    enum class AssetType
    {
        Texture,
        EnvironmentTexture,
        Shader,
        AudioClip,
        AudioStream,
        StaticMesh,
        SkinnedMesh,
        Material,
        Scene,
        Entity,
        Font
    };

    struct Asset : Object
    {
        LUMA_CLASS(Asset, Object)
    public:
        ~Asset() override = default;
        virtual AssetType getAssetType() const = 0;
    };
}