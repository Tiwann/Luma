#pragma once
#include "Luma/Containers/Array.h"
#include "Luma/Containers/String.h"
#include "ShaderDataType.h"
#include "VertexInputRate.h"
#include "Luma/Containers/HashMap.h"

namespace Luma
{
    struct VertexAttribute
    {
        String name;
        ShaderDataType type;
        uint32_t binding;
        bool operator==(const VertexAttribute&) const = default;
    };

    class VertexInputLayout
    {
    public:
        VertexInputLayout() = default;

        void addInputBinding(uint32_t binding, VertexInputRate inputRateBinding);
        void addInputAttribute(const VertexAttribute& attribute);
        void addInputAttribute(String name, ShaderDataType type, uint32_t binding);

        uint32_t getStride(uint32_t binding) const;
        uint32_t getAttributeCount() const;
        uint32_t getBindingCount() const;
        uint32_t getAttributeOffset(const VertexAttribute& attribute) const;
        uint32_t getAttributeOffset(const String& name) const;
        uint32_t getAttributeOffset(uint32_t index) const;

        const Array<VertexAttribute>& getInputAttributes() const;
        const HashMap<uint32_t, VertexInputRate>& getInputBindings() const;

        bool operator==(const VertexInputLayout& other) const;
    private:
        HashMap<uint32_t, VertexInputRate> m_InputBindings;
        Array<VertexAttribute> m_InputAttributes;
    };
}
