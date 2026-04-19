#pragma once
#include "ShaderStage.h"

namespace Luma
{
    struct FShaderCode
    {
        EShaderStageBits shaderStage = EShaderStageBits::None;
        const uint8_t* data = nullptr;
        uint64_t dataSize = 0;
        const uint8_t* reflectionData = nullptr;
        uint64_t reflectionDataSize = 0;
    };

    struct FShaderProgramDesc
    {
        uint32_t shaderCount = 0;
        const FShaderCode* shaders = nullptr;
    };

    struct IShaderProgram
    {
        IShaderProgram() = default;
        virtual ~IShaderProgram() = default;

        virtual bool initialize(const FShaderProgramDesc& shaderDesc) = 0;
        virtual void destroy() = 0;
        virtual FShaderStageFlags getStageFlags() { return EShaderStageBits::None; }
    };
}