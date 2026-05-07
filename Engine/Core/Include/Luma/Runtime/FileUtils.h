#pragma once
#include "Luma/Containers/String.h"
#include "Luma/Containers/StringView.h"
#include "Luma/IO/Stream.h"

namespace Luma::FileUtils
{
    TArray<uint8_t> readToBuffer(FStringView filepath);
    FString readToString(FStringView filepath);
    TArray<uint8_t> readTillEnd(FStream& stream);
}
