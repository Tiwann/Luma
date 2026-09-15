#pragma once
#include "Luma/Rendering/BindingType.h"
#include "Luma/Rendering/ShaderStage.h"
#include "SlangFwd.h"

namespace Luma
{
    EShaderStages getStage(SlangStage stage);
    EBindingType getBindingType(slang::BindingType bindingType);
}

