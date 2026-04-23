#pragma once
#include "Pair.h"
#include <unordered_map>

namespace Luma
{
    template<typename K, typename V, typename P = TPair<K, V>, typename H = std::unordered_map<K, V>>
    class HashMap
    {
    public:
        using SizeType = uint64_t;
        HashMap() = default;

        V& operator[](const K& key) { return m_Data[key]; }
        V& operator[](K&& key) { return m_Data[key]; }

        SizeType count() const { return m_Data.size(); }
        void set(const K& key, const V& value) { m_Data[key] = value; }
        void set(const P& pair) { m_Data[pair.key] = pair.value; }
        V& at(const K& key) { return m_Data.at(key); }
        const V& at(const K& key) const { return m_Data.at(key); }
        bool contains(const K& key) { return m_Data.contains(key); }
        void removeAt(SizeType index) { m_Data.erase(m_Data.begin() + index); }
        void remove(const K& key) { m_Data.erase(key); }
        SizeType find(const K& key) { return m_Data.find(key); }

    private:
        H m_Data;
    };
}