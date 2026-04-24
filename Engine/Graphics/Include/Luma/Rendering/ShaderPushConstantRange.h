#pragma once
#include "ShaderStage.h"

namespace Luma
{
    struct FShaderPushConstantRange
    {
        uint64_t offset = 0;
        uint64_t size = 0;
        EShaderStageBits stage = EShaderStageBits::None;
    };
}
