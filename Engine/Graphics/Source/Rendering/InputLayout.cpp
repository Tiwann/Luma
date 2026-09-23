#include "Luma/Rendering/InputLayout.h"

namespace Luma
{
    template<>
    struct THasher<VertexAttribute>
    {
        uint64_t operator()(const VertexAttribute& attr) const noexcept
        {
            uint64_t seed = 0;
            auto hashCombine = [&seed]<typename T>(const T& value)
            {
                seed ^= THasher<std::decay_t<T>>{}(value)
                      + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            };

            hashCombine(attr.name);
            hashCombine(static_cast<std::underlying_type_t<ShaderDataType>>(attr.type));
            hashCombine(attr.binding);

            return seed;
        }
    };

    template<>
    struct THasher<VertexInputLayout>
    {
        uint64_t operator()(const VertexInputLayout& layout) const noexcept
        {
            size_t seed = 0;
            auto hashCombine = [&seed]<typename T>(const T& value)
            {
                seed ^= THasher<std::decay_t<T>>{}(value)
                      + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            };

            for (const VertexAttribute& attr : layout.getInputAttributes())
                hashCombine(attr);

            for (const auto& [binding, inputRate] : layout.getInputBindings())
            {
                hashCombine(binding);
                hashCombine(static_cast<std::underlying_type_t<VertexInputRate>>(inputRate));
            }

            return seed;
        }
    };

    void VertexInputLayout::addInputBinding(const uint32_t binding, const VertexInputRate inputRateBinding)
    {
        m_InputBindings[binding] = inputRateBinding;
    }

    void VertexInputLayout::addInputAttribute(const VertexAttribute& attribute)
    {
        m_InputAttributes.add(attribute);
    }

    void VertexInputLayout::addInputAttribute(FString name, ShaderDataType type, uint32_t binding)
    {
        m_InputAttributes.add(VertexAttribute{name, type, binding});
    }

    uint32_t VertexInputLayout::getStride(const uint32_t binding) const
    {
        uint32_t result = 0;
        for (const VertexAttribute& attribute : m_InputAttributes)
        {
            if (attribute.binding == binding)
                result += getDataTypeSize(attribute.type);
        }
        return result;
    }

    uint32_t VertexInputLayout::getAttributeCount() const
    {
        return m_InputAttributes.count();
    }

    uint32_t VertexInputLayout::getBindingCount() const
    {
        return m_InputBindings.count();
    }

    uint32_t VertexInputLayout::getAttributeOffset(const VertexAttribute& attribute) const
    {
        const auto index = m_InputAttributes.find(attribute);
        LUMA_ASSERT(index != -1, "Invalid vertex attribute!");
        uint32_t result = 0;
        for (size_t i = 0; i < index; i++)
            result += getDataTypeSize(m_InputAttributes[i].type);
        return result;
    }

    uint32_t VertexInputLayout::getAttributeOffset(const FString& name) const
    {
        const auto predicate = [&name](const VertexAttribute& attribute) { return attribute.name == name; };
        const VertexAttribute* attribute = m_InputAttributes.single(predicate);
        if (!attribute) return -1u;
        LUMA_ASSERT(attribute, "Invalid vertex attribute!");
        const auto index = m_InputAttributes.find(*attribute);
        uint32_t result = 0;
        for (size_t i = 0; i < index; i++)
            result += getDataTypeSize(m_InputAttributes[i].type);
        return result;
    }

    uint32_t VertexInputLayout::getAttributeOffset(const uint32_t index) const
    {
        LUMA_ASSERT(index < m_InputAttributes.count(), "Invalid vertex attribute!");
        uint32_t result = 0;
        for (size_t i = 0; i < index; i++)
            result += getDataTypeSize(m_InputAttributes[i].type);
        return result;
    }

    const TArray<VertexAttribute>& VertexInputLayout::getInputAttributes() const
    {
        return m_InputAttributes;
    }

    const THashMap<uint32_t, VertexInputRate>& VertexInputLayout::getInputBindings() const
    {
        return m_InputBindings;
    }

    bool VertexInputLayout::operator==(const VertexInputLayout& other) const
    {
        return m_InputAttributes == other.m_InputAttributes && m_InputBindings == other.m_InputBindings;
    }
}
