#pragma once
#include "Types.h"
#include "Luma/Containers/StringView.h"

namespace Luma
{
    class Subsystem
    {

    };

    class AssetSubsystem : public Subsystem
    {
    public:
        using AssetCategory = uint32_t;
        using AssetType = uint32_t;

        AssetType registerAssetType(StringView className)
    private:
    };
}
