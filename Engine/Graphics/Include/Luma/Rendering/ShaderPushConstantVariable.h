#pragma once
#include "ShaderStage.h"

namespace Luma
{
    struct ShaderPushConstantVariable
    {
        FString name;
        int64_t offset = 0;
        uint64_t size = 0;
        ShaderStageFlags visibility = ShaderStage::None;
    };
}
