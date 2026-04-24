#pragma once
#include "Luma/Containers/String.h"

namespace Luma
{
    struct FShaderCompileDefine
    {
        FString key;
        FString value;
        bool operator==(const FShaderCompileDefine&) const = default;
    };
}
