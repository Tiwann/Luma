#pragma once
#include "ShaderStage.h"

namespace Luma
{
    struct ShaderPushConstantVariable
    {
        String name;
        int64_t offset = 0;
        uint64_t size = 0;
        ShaderStageFlags visibility = ShaderStage::None;
    };
}
