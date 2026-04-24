#pragma once
#include "Luma/Containers/String.h"
#include "BindingType.h"
#include "ShaderStage.h"

namespace Luma
{
    struct FShaderBinding
    {
        FString name;
        EBindingType bindingType = EBindingType::None;
        FShaderStageFlags stageFlags = EShaderStageBits::None;
        uint32_t bindingIndex = 0;
        uint32_t descriptorCount = 0;
        bool operator==(const FShaderBinding& other) const = default;
    };
}