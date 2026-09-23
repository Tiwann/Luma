#pragma once
#include "Luma/Rendering/BindingType.h"
#include "Luma/Rendering/ShaderStage.h"
#include <spirv_reflect.h>

namespace Luma
{
    BindingType getBindingType(SpvReflectDescriptorType type);
    ShaderStage getShaderStage(SpvReflectShaderStageFlagBits bits);
}
