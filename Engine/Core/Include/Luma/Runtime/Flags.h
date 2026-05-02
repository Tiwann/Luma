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

    template<typename Enum>
    class TFlags
    {
    public:
        using IntegerType = std::underlying_type_t<Enum>;

        constexpr TFlags() = default;
        constexpr TFlags(Enum value) : m_Value(value) {}
        constexpr TFlags(IntegerType value) : m_Value((Enum)value) {}

        static constexpr TFlags all() { return Flags(static_cast<Enum>(~static_cast<IntegerType>(0))); }
        static constexpr TFlags none(){ return Flags(static_cast<Enum>(0)); }

        constexpr operator IntegerType() const { return (IntegerType)m_Value; }
        constexpr explicit operator Enum() const { return (Enum)m_Value; }

        constexpr bool operator==(const TFlags other) const { return m_Value == other.m_Value; }
        constexpr bool operator==(const Enum other) const { return m_Value == other; }
        constexpr bool operator==(IntegerType other) const { return (IntegerType)m_Value == other; }

        constexpr TFlags operator~() const {return Flags(static_cast<Enum>(~static_cast<IntegerType>(m_Value))); }
        constexpr TFlags operator|(const Enum other) const { return TFlags((IntegerType)m_Value | (IntegerType)other); };
        constexpr TFlags operator&(const Enum other) const { return TFlags((IntegerType)m_Value & (IntegerType)other); };
        constexpr TFlags operator^(const Enum other) const { return TFlags((IntegerType)m_Value ^ (IntegerType)other); };
        constexpr TFlags& operator|=(const Enum other) { m_Value = (Enum)((IntegerType)m_Value | (IntegerType)other); return *this; };
        constexpr TFlags& operator&=(const Enum other) { m_Value = (Enum)((IntegerType)m_Value & (IntegerType)other); return *this; };
        constexpr TFlags& operator^=(const Enum other) { m_Value = (Enum)((IntegerType)m_Value ^ (IntegerType)other); return *this; };
        constexpr TFlags operator|(const TFlags other) const { return TFlags(static_cast<Enum>(m_Value | other.m_Value)); }
        constexpr TFlags operator&(const TFlags other) const { return TFlags(static_cast<Enum>(m_Value & other.m_Value)); }
        constexpr TFlags operator^(const TFlags other) const { return TFlags(static_cast<Enum>(m_Value ^ other.m_Value)); }

        template<typename T> requires std::is_convertible_v<T, IntegerType>
        T as() const { return (T)m_Value; }

        constexpr bool contains(const Enum other) const { return ((IntegerType)m_Value & (IntegerType)other) != 0; }

        constexpr TFlags& append(const Enum value)
        {
            m_Value = (Enum)((IntegerType)m_Value | (IntegerType)value);
            return *this;
        }

        constexpr TFlags& remove(const Enum value)
        {
            m_Value = (Enum)((IntegerType)m_Value & ~(IntegerType)value);
            return *this;
        }

        constexpr TFlags& toggle(const Enum value) { m_Value ^= static_cast<IntegerType>(value); return *this; }
    private:
        Enum m_Value = (Enum)0;
    };

    template<typename Enum> requires std::is_scoped_enum_v<Enum>
    constexpr TFlags<Enum> operator|(const Enum lhs, const Enum rhs)
    {
        using IntegerType = std::underlying_type_t<Enum>;
        return Flags((Enum)((IntegerType)lhs | (IntegerType)rhs));
    }

    template<typename Enum> requires std::is_scoped_enum_v<Enum>
    constexpr TFlags<Enum> operator&(const Enum lhs, const Enum rhs)
    {
        using IntegerType = std::underlying_type_t<Enum>;
        return Flags((Enum)((IntegerType)lhs & (IntegerType)rhs));
    }

    template<typename Enum> requires std::is_scoped_enum_v<Enum>
    constexpr TFlags<Enum> operator^(const Enum lhs, const Enum rhs)
    {
        using IntegerType = std::underlying_type_t<Enum>;
        return Flags((Enum)((IntegerType)lhs ^ (IntegerType)rhs));
    }
}
