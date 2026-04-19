#pragma once
#include "Luma/Containers/Array.h"
#include "BindingType.h"
#include "ShaderStage.h"
#include <cstdint>


namespace Luma
{
    static constexpr uint32_t INVALID_BINDING_INDEX = ~0u;
    static constexpr uint32_t INVALID_SET_INDEX = ~0u;

    struct FShaderBinding
    {
        const char* name = nullptr;
        EBindingType bindingType = EBindingType::None;
        FShaderStageFlags stageFlags = EShaderStageBits::None;
        uint32_t bindingIndex = INVALID_BINDING_INDEX;
        uint32_t descriptorCount = 0;
    };

    struct FBindingSetLayoutDesc
    {
        struct IRenderDevice* device = nullptr;
        uint32_t setIndex = INVALID_SET_INDEX;
        uint32_t bindingCount = 0;
        const FShaderBinding* bindings = nullptr;
    };

    struct IBindingSetLayout
    {
        IBindingSetLayout() = default;
        virtual ~IBindingSetLayout() = default;

        virtual bool initialize(const FBindingSetLayoutDesc& layoutDesc) = 0;
        virtual void destroy() = 0;
    };
}
