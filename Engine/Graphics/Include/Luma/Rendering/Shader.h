#pragma once
#include "Luma/Memory/RefCounted.h"
#include "Luma/Containers/ArrayView.h"
#include "Luma/Containers/HashMap.h"
#include "ShaderStage.h"
#include "ShaderBinding.h"
#include "ShaderPushConstantVariable.h"

namespace Luma
{
    struct IGPUDevice;
    struct IBindingGroup;

    struct FShaderCode
    {
        EShaderStage stage;
        TArrayView<uint8_t> code;
    };

    struct FShaderDesc
    {
        IGPUDevice* device = nullptr;
        TArrayView<FShaderCode> shaderCodes;
    };

    struct IShader : IRefCounted<IShader>
    {
        IShader() = default;
        ~IShader() override = default;

        virtual bool initialize(const FShaderDesc& desc) = 0;
        virtual void destroy() = 0;

        virtual IBindingGroup* createBindingGroup(uint32_t groupIndex);

        IGPUDevice* getDevice() const { return m_Device; }
        FShaderStageFlags getStages() const { return m_Stages; }
        const auto& getBindings() const { return m_Bindings; }
        uint32_t getBindingFromName(const FString& name) const { return m_NameToBindingCache[name]; }
        const auto& getPushConstantVariables() const { return m_PushConstantsVars; }
    protected:
        IGPUDevice* m_Device = nullptr;
        FShaderStageFlags m_Stages = EShaderStage::None;
        THashMap<uint32_t, THashMap<uint32_t, FShaderBinding>> m_Bindings;
        THashMap<FString, uint32_t> m_NameToBindingCache;
        TArray<FShaderPushConstantVariable> m_PushConstantsVars;
    };
}
