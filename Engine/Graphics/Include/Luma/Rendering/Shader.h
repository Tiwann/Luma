#pragma once
#include "Luma/Memory/RefCounted.h"
#include "Luma/Containers/ArrayView.h"
#include "Luma/Containers/HashMap.h"
#include "ShaderStage.h"
#include "ShaderBinding.h"
#include "ShaderPushConstantVariable.h"

namespace Luma::RHI
{
    struct Device;
    struct BindingGroup;

    struct ShaderCode
    {
        ShaderStage stage;
        TArrayView<uint8_t> code;
    };

    struct ShaderDesc
    {
        Device* device = nullptr;
        TArrayView<ShaderCode> shaderCodes;
    };

    struct Shader : RefCounted<Shader>
    {
        Shader() = default;
        ~Shader() override = default;

        virtual bool initialize(const ShaderDesc& desc) = 0;
        virtual void destroy() = 0;

        virtual BindingGroup* createBindingGroup(uint32_t groupIndex);

        Device* getDevice() const { return m_Device; }
        ShaderStageFlags getStages() const { return m_Stages; }
        const auto& getBindings() const { return m_Bindings; }
        uint32_t getBindingFromName(const FString& name) const { return m_NameToBindingCache[name]; }
        const auto& getPushConstantVariables() const { return m_PushConstantsVars; }
    protected:
        Device* m_Device = nullptr;
        ShaderStageFlags m_Stages = ShaderStage::None;
        THashMap<uint32_t, THashMap<uint32_t, ShaderBinding>> m_Bindings;
        THashMap<FString, uint32_t> m_NameToBindingCache;
        TArray<ShaderPushConstantVariable> m_PushConstantsVars;
    };
}
