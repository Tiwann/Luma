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
        FShaderStageFlags visibility = EShaderStage::None;
        uint32_t bindingIndex = 0;
        uint32_t arrayCount = 0;
    };
}