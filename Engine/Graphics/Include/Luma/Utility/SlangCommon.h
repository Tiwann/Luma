#pragma once
#include "Luma/Rendering/BindingType.h"
#include "Luma/Rendering/ShaderCompileTarget.h"
#include "Luma/Rendering/ShaderStage.h"
#include "SlangFwd.h"
#include <slang/slang.h>


namespace Slang
{
    template<typename T>
    class ComPtr;
}

namespace slang
{
    using GlobalSessionHandle = Slang::ComPtr<IGlobalSession>;
    using SessionHandle = Slang::ComPtr<ISession>;
    using EntryPointHandle = Slang::ComPtr<IEntryPoint>;
    using ComponentHandle = Slang::ComPtr<IComponentType>;
    using BlobHandle = Slang::ComPtr<IBlob>;
}

namespace Luma
{
    SlangCompileTarget getCompileTarget(EShaderCompileTarget target);
    EShaderStageBits getStage(SlangStage stage);
    EBindingType getBindingType(slang::BindingType bindingType);
}

