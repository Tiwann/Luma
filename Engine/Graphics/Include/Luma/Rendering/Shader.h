#pragma once
#include "ShaderStage.h"
#include "Luma/Containers/String.h"
#include "Luma/Memory/RefCounted.h"
#include "Luma/Runtime/Asset.h"

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

    struct IShader : IAsset, IRefCounted<IShader>
    {
        IShader() = default;
        ~IShader() override = default;
        EAssetType getAssetType() const final { return EAssetType::Shader; }

        virtual bool initialize(const FShaderDesc& shaderDesc) = 0;
        virtual void destroy() = 0;
        virtual IBindingSet* createBindingSet(uint32_t setIndex) = 0;
    };
}
