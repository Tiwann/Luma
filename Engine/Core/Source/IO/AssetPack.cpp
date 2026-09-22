#include "Luma/IO/AssetPack.h"

namespace Luma
{
    struct FAssetPackHeader
    {
        uint8_t magic[4] = {'L', 'M', 'P', 'K'};
        uint32_t assetEntryCount = 0;
        uint32_t assetEntriesOffset = 0;
    };


    void FAssetPackSerializer::serialize(IStream& stream, const FAssetPack& assetPack)
    {
        FAssetPackHeader header;
        stream.writeObject(header);
    }

    void FAssetPackSerializer::deserialize(IStream& stream, FAssetPack& assetPack)
    {
    }
}