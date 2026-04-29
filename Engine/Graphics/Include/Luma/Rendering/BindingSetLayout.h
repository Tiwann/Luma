#pragma once
#include "ShaderBinding.h"
#include <cstdint>


namespace Luma
{
    struct IRenderDevice;

    struct FBindingSetLayoutDesc
    {
        IRenderDevice* device = nullptr;
        uint32_t setIndex = 0;
        TArray<FShaderBinding> bindings;
    };

    struct IBindingSetLayout
    {
        IBindingSetLayout() = default;
        virtual ~IBindingSetLayout() = default;

        virtual bool initialize(const FBindingSetLayoutDesc& layoutDesc) = 0;
        virtual void destroy() = 0;

        uint32_t getSetIndex() const { return m_SetIndex; }
        const TArray<FShaderBinding>& getBindings() const { return m_Bindings; }
    protected:
        uint32_t m_SetIndex = 0;
        TArray<FShaderBinding> m_Bindings;
    };
}
