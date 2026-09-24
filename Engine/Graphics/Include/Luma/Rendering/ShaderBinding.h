#pragma once
#include "Luma/Containers/String.h"
#include "BindingType.h"
#include "ShaderStage.h"

namespace Luma
{
    struct ShaderBinding
    {
        String name;
        BindingType bindingType = BindingType::None;
        ShaderStageFlags visibility = ShaderStage::None;
        uint32_t bindingIndex = 0;
        uint32_t arrayCount = 0;
    };
}