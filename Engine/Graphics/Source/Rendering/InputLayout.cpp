#include "Luma/Rendering/InputLayout.h"

namespace Luma
{
    void FVertexInputLayout::addInputBinding(const uint32_t binding, const EVertexInputRate inputRateBinding)
    {
        m_InputBindings[binding] = inputRateBinding;
    }

    void FVertexInputLayout::addInputAttribute(const FVertexAttribute& attribute)
    {
        m_InputAttributes.add(attribute);
    }
    
    uint32_t FVertexInputLayout::getStride(const uint32_t binding) const
    {
        uint32_t result = 0;
        for (const FVertexAttribute& attribute : m_InputAttributes)
        {
            if (attribute.binding == binding)
                result += getDataTypeSize(attribute.type);
        }
        return result;
    }

    uint32_t FVertexInputLayout::getAttributeCount() const
    {
        return m_InputAttributes.count();
    }

    uint32_t FVertexInputLayout::getBindingCount() const
    {
        return m_InputBindings.count();
    }

    uint32_t FVertexInputLayout::getAttributeOffset(const FVertexAttribute& attribute) const
    {
        const auto index = m_InputAttributes.find(attribute);
        LUMA_ASSERT(index != -1, "Invalid vertex attribute!");
        uint32_t result = 0;
        for (size_t i = 0; i < index; i++)
            result += getDataTypeSize(m_InputAttributes[i].type);
        return result;
    }

    uint32_t FVertexInputLayout::getAttributeOffset(const FString& name) const
    {
        const auto predicate = [&name](const FVertexAttribute& attribute) { return attribute.name == name; };
        const FVertexAttribute* attribute = m_InputAttributes.single(predicate);
        if (!attribute) return -1u;
        LUMA_ASSERT(attribute, "Invalid vertex attribute!");
        const auto index = m_InputAttributes.find(*attribute);
        uint32_t result = 0;
        for (size_t i = 0; i < index; i++)
            result += getDataTypeSize(m_InputAttributes[i].type);
        return result;
    }

    uint32_t FVertexInputLayout::getAttributeOffset(const uint32_t index) const
    {
        LUMA_ASSERT(index < m_InputAttributes.count(), "Invalid vertex attribute!");
        uint32_t result = 0;
        for (size_t i = 0; i < index; i++)
            result += getDataTypeSize(m_InputAttributes[i].type);
        return result;
    }

    const TArray<FVertexAttribute>& FVertexInputLayout::getInputAttributes() const
    {
        return m_InputAttributes;
    }

    const THashMap<uint32_t, EVertexInputRate>& FVertexInputLayout::getInputBindings() const
    {
        return m_InputBindings;
    }
}
