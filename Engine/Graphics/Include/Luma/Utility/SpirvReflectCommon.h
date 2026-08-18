#pragma once
#include "Luma/Rendering/BindingType.h"
#include "Luma/Rendering/ShaderStage.h"
#include <spirv_reflect.h>

namespace Luma
{
    EBindingType getBindingType(SpvReflectDescriptorType type);
    EShaderStageBits getShaderStage(SpvReflectShaderStageFlagBits bits);
}
