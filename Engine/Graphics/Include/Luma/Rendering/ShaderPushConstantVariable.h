#pragma once
#include "ShaderStage.h"

namespace Luma
{
    struct FShaderPushConstantVariable
    {
        FString name;
        int64_t offset = 0;
        uint64_t size = 0;
        FShaderStageFlags visibility = EShaderStage::None;
    };
}
