#pragma once
#include "ShaderStage.h"
#include "Luma/Containers/String.h"
#include "Luma/Memory/RefCounted.h"

namespace Luma
{
    struct IRenderDevice;
    struct IBindingSet;

    struct FShaderDesc
    {
        IRenderDevice* device = nullptr;
        FShaderStageFlags stageFlags = EShaderStageBits::None;
        FString moduleName = FString();
        FString filepath = FString();
    };

    struct IShader : IRefCounted<IShader>
    {
        IShader() = default;
        ~IShader() override = default;

        virtual bool initialize(const FShaderDesc& shaderDesc) = 0;
        virtual void destroy() = 0;
        virtual IBindingSet* createBindingSet(uint32_t setIndex) const = 0;

        FShaderStageFlags getStageFlags() const { return m_StageFlags; }
    protected:
        FShaderStageFlags m_StageFlags = EShaderStageBits::None;
    };
}
