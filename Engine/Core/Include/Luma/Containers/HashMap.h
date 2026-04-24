#pragma once
#include "Pair.h"
#include <unordered_map>

namespace Luma
{
    template<typename Key, typename Value, typename Hasher = std::hash<Key>, typename MapImpl = std::unordered_map<Key, Value, Hasher>>
    class THashMap
    {
    public:
        using SizeType = uint64_t;
        THashMap() = default;

        Value& operator[](const Key& key) { return m_Data[key]; }
        Value& operator[](Key&& key) { return m_Data[key]; }

        SizeType count() const { return m_Data.size(); }
        void set(const Key& key, const Value& value) { m_Data[key] = value; }
        Value& at(const Key& key) { return m_Data.at(key); }
        const Value& at(const Key& key) const { return m_Data.at(key); }
        bool contains(const Key& key) { return m_Data.contains(key); }
        void removeAt(SizeType index) { m_Data.erase(m_Data.begin() + index); }
        void remove(const Key& key) { m_Data.erase(key); }
        SizeType find(const Key& key) { return m_Data.find(key); }

        bool operator==(const THashMap& other) const
        {
            return m_Data == other.m_Data;
        }
    private:
        MapImpl m_Data;
    };
}