#pragma once
#include "Luma/Containers/Array.h"
#include "Luma/Containers/String.h"
#include "ShaderDataType.h"
#include "VertexInputRate.h"
#include "Luma/Containers/HashMap.h"

namespace Luma
{
    struct FVertexAttribute
    {
        FString name;
        EShaderDataType type;
        uint32_t binding;
        bool operator==(const FVertexAttribute&) const = default;
    };

    class FVertexInputLayout
    {
    public:
        FVertexInputLayout() = default;

        void addInputBinding(uint32_t binding, EVertexInputRate inputRateBinding);
        void addInputAttribute(const FVertexAttribute& attribute);

        uint32_t getStride(uint32_t binding) const;
        uint32_t getAttributeCount() const;
        uint32_t getBindingCount() const;
        uint32_t getAttributeOffset(const FVertexAttribute& attribute) const;
        uint32_t getAttributeOffset(const FString& name) const;
        uint32_t getAttributeOffset(uint32_t index) const;

        const TArray<FVertexAttribute>& getInputAttributes() const;
        const THashMap<uint32_t, EVertexInputRate>& getInputBindings() const;
    private:
        THashMap<uint32_t, EVertexInputRate> m_InputBindings;
        TArray<FVertexAttribute> m_InputAttributes;
    };
}
