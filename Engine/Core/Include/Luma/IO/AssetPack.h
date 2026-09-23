#pragma once
#include "Luma/IO/Serializer.h"
#include "Luma/Runtime/Asset.h"

namespace Luma
{
    struct FAssetEntry
    {
        uint64_t id;
        AssetType type;
        uint64_t offset;

    };


    class FAssetPack
    {
    public:

    private:
    };

    struct FAssetPackSerializer : ISerializer<FAssetPack>
    {
        void serialize(IStream& stream, const FAssetPack& assetPack) override;
        void deserialize(IStream& stream, FAssetPack& assetPack) override;
    };
}
