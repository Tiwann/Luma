#pragma once
#include "Luma/Rendering/BindingType.h"
#include "Luma/Rendering/ShaderCompileTarget.h"
#include "Luma/Rendering/ShaderCompileDefine.h"
#include "Luma/Rendering/ShaderStage.h"
#include "SlangFwd.h"

enum SlangStage : uint32_t;
enum SlangCompileTarget : int;


namespace Luma
{
    SlangCompileTarget getCompileTarget(EShaderCompileTarget target);
    EShaderStageBits getStage(SlangStage stage);
    EBindingType getBindingType(slang::BindingType bindingType);
    slang::PreprocessorMacroDesc toPreprocessorMacroDesc(const FShaderCompileDefine& define);
}

