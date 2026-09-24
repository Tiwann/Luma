#pragma once
#include "Luma/Containers/String.h"
#include "Luma/Containers/StringView.h"
#include "Luma/IO/Stream.h"

namespace Luma::FileUtils
{
    Array<uint8_t> readToBuffer(StringView filepath);
    String readToString(StringView filepath);
    Array<uint8_t> readTillEnd(Stream& stream);
}
