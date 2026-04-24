#pragma once
#include "Luma/Math/Concepts.h"
#include <cstdint>
#include <type_traits>

namespace Luma
{
    template<IntegerType T = uint32_t>
    consteval T BIT(const T n)
    {
        return T(1) << n;
    }

    template<typename Enum> requires std::is_scoped_enum_v<Enum>
    class Flags
    {
    public:
        using IntegerType = std::underlying_type_t<Enum>;

        constexpr Flags() = default;
        constexpr Flags(Enum value) : m_Value(value) {}
        constexpr Flags(IntegerType value) : m_Value((Enum)value) {}

        static constexpr Flags all() {  return ~0; }
        static constexpr Flags none() { return 0; }

        constexpr operator IntegerType() const { return (IntegerType)m_Value; }
        constexpr explicit operator Enum() const { return (Enum)m_Value; }

        constexpr bool operator==(const Flags& other) const { return m_Value == other.m_Value; }
        constexpr bool operator==(const Enum& other) const { return m_Value == other; }
        constexpr bool operator==(IntegerType other) const { return (IntegerType)m_Value == other; }

        constexpr Flags operator|(const Enum& other) const { return Flags((IntegerType)m_Value | (IntegerType)other); };
        constexpr Flags operator&(const Enum& other) const { return Flags((IntegerType)m_Value & (IntegerType)other); };
        constexpr Flags operator^(const Enum& other) const { return Flags((IntegerType)m_Value ^ (IntegerType)other); };
        constexpr Flags& operator|=(const Enum& other) { m_Value = (Enum)((IntegerType)m_Value | (IntegerType)other); return *this; };
        constexpr Flags& operator&=(const Enum& other) { m_Value = (Enum)((IntegerType)m_Value & (IntegerType)other); return *this; };
        constexpr Flags& operator^=(const Enum& other) { m_Value = (Enum)((IntegerType)m_Value ^ (IntegerType)other); return *this; };

        template<typename T> requires std::is_convertible_v<T, IntegerType>
        T as() const { return (T)m_Value; }

        constexpr bool contains(const Enum& other) const { return ((IntegerType)m_Value & (IntegerType)other) != 0; }

        constexpr Flags& append(const Enum value)
        {
            m_Value = (Enum)((IntegerType)m_Value | (IntegerType)value);
            return *this;
        }

        constexpr Flags& remove(const Enum value)
        {
            m_Value = (Enum)((uint32_t)m_Value & ~(uint32_t)value);
            return *this;
        }

        constexpr Flags& toggle(const Enum value) { return contains(value) ? remove(value) : append(value); }
    private:
        Enum m_Value = (Enum)0;
    };

    template<typename Enum> requires std::is_scoped_enum_v<Enum>
    constexpr Flags<Enum> operator|(const Enum& lhs, const Enum& rhs)
    {
        using IntegerType = std::underlying_type_t<Enum>;
        return Flags((Enum)((IntegerType)lhs | (IntegerType)rhs));
    }

    template<typename Enum> requires std::is_scoped_enum_v<Enum>
    constexpr Flags<Enum> operator&(const Enum& lhs, const Enum& rhs)
    {
        using IntegerType = std::underlying_type_t<Enum>;
        return Flags((Enum)((IntegerType)lhs & (IntegerType)rhs));
    }

    template<typename Enum> requires std::is_scoped_enum_v<Enum>
    constexpr Flags<Enum> operator^(const Enum& lhs, const Enum& rhs)
    {
        using IntegerType = std::underlying_type_t<Enum>;
        return Flags((Enum)((IntegerType)lhs ^ (IntegerType)rhs));
    }
}
