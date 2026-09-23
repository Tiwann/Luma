#pragma once
#include "Object.h"

namespace Luma
{
    enum class AssetType
    {
        Texture,
        AudioClip,
        AudioStream,
        Shader,
        StaticMesh,
        SkinnedMesh,
        Material,
        Scene,
        Entity,
        Font
    };

    struct Asset : Object
    {
        ~Asset() override = default;
        virtual AssetType getAssetType() const = 0;
    };
}