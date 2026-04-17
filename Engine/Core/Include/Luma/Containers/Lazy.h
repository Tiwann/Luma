#pragma once
#include <functional>

namespace Luma
{
    template<typename T>
    class TLazy
    {
    public:
        constexpr TLazy() = default;
        constexpr explicit TLazy(const T& value) : m_Value(value), m_Dirty(false) {};

        const T& get(const std::function<T()>& getter)
        {
            if (!m_Dirty) return m_Value;

            m_Value = getter();
            m_Dirty = false;
            return m_Value;
        }

        void setDirty() { m_Dirty = true;}
        bool isDirty() const { return m_Dirty; }

        const T* getAddressOf() const { return &m_Value; }
        const T& getRef() const { return m_Value; }

    private:
        T m_Value;
        bool m_Dirty;
    };
}