#pragma once
#include "Pair.h"
#include "Array.h"
#include "Luma/Runtime/Assertion.h"
#include "Luma/Math/Concepts.h"
#include "Luma/Runtime/Hash.h"
#include <cstdint>


namespace Luma
{
    template<typename Key>
    struct Hasher;

    template<typename Key, typename Value, typename HasherType = Hasher<Key>>
    class HashMap final
    {
    public:
        using SizeType = uint64_t;
        using PairType = Pair<Key, Value>;
        using BucketType = Array<PairType>;

        struct Iterator
        {
            Array<BucketType>* buckets = nullptr;
            SizeType bucketIdx = 0;
            SizeType pairIdx = 0;

            void normalise()
            {
                while (bucketIdx < buckets->count())
                {
                    if (pairIdx < (*buckets)[bucketIdx].count())
                        return;
                    ++bucketIdx;
                    pairIdx = 0;
                }
            }

            PairType& operator*() { return (*buckets)[bucketIdx][pairIdx]; }
            PairType* operator->() { return &(**this); }

            Iterator& operator++()
            {
                ++pairIdx;
                normalise();
                return *this;
            }

            bool operator==(const Iterator& other) const
            {
                return bucketIdx == other.bucketIdx && pairIdx == other.pairIdx;
            }

            bool operator!=(const Iterator& other) const { return !(*this == other); }
        };

        struct ConstIterator
        {
            const Array<BucketType>* buckets = nullptr;
            SizeType bucketIdx = 0;
            SizeType pairIdx = 0;

            void normalise()
            {
                while (bucketIdx < buckets->count())
                {
                    if (pairIdx < (*buckets)[bucketIdx].count())
                        return;
                    ++bucketIdx;
                    pairIdx = 0;
                }
            }

            const PairType& operator*() const { return (*buckets)[bucketIdx][pairIdx]; }
            const PairType* operator->() const { return &(**this); }

            ConstIterator& operator++()
            {
                ++pairIdx;
                normalise();
                return *this;
            }

            bool operator==(const ConstIterator& other) const
            {
                return bucketIdx == other.bucketIdx && pairIdx == other.pairIdx;
            }

            bool operator!=(const ConstIterator& other) const { return !(*this == other); }
        };

        HashMap()
        {
            for (SizeType i = 0; i < 16; ++i)
                m_Buckets.emplace(BucketType{});
        }

        explicit HashMap(SizeType bucketCount)
        {
            for (SizeType i = 0; i < bucketCount; ++i)
                m_Buckets.emplace(BucketType{});
        }

        Value& operator[](const Key& key) { return getOrInsert(key); }
        Value& operator[](Key&& key) { return getOrInsert(std::move(key)); }

        const Value& operator[](const Key& key) const
        {
            const PairType* found = find(key);
            LUMA_ASSERT(found, "THashMap::at — key not found");
            return found->value;
        }

        Value& at(const Key& key)
        {
            return operator[](key);
        }

        const Value& at(const Key& key) const
        {
            return operator[](key);
        }

        SizeType count() const { return m_Count; }
        bool contains(const Key& key) const { return find(key) != nullptr; }
        void set(const Key& key, const Value& value) { getOrInsert(key) = value; }
        void remove(const Key& key)
        {
            BucketType& bucket = m_Buckets[getBucketIndex(key)];

            for (SizeType i = 0; i < bucket.count(); ++i)
            {
                if (bucket[i].key == key)
                {
                    bucket.removeAt(i);
                    --m_Count;
                    return;
                }
            }
        }

        Value& emplace(const Key& key) { return getOrInsert(key); }
        Value& emplace(Key&& key) { return getOrInsert(std::move(key)); }

        Iterator begin()
        {
            Iterator it{&m_Buckets, 0, 0};
            it.normalise();
            return it;
        }

        ConstIterator begin() const
        {
            ConstIterator it{&m_Buckets, 0, 0};
            it.normalise();
            return it;
        }

        Iterator end() { return {&m_Buckets, m_Buckets.count(), 0}; }
        ConstIterator end() const { return {&m_Buckets, m_Buckets.count(), 0}; }

        Value* findValue(const Key& key)
        {
            PairType* pair = find(key);
            if (!pair) return nullptr;
            return &pair->value;
        }

        const Value* findValue(const Key& key) const
        {
            PairType* pair = find(key);
            if (!pair) return nullptr;
            return &pair->value;
        }

        PairType* find(const Key& key)
        {
            BucketType& bucket = m_Buckets[getBucketIndex(key)];
            for (PairType& pair : bucket)
                if (pair.key == key)
                    return &pair;
            return nullptr;
        }

        const PairType* find(const Key& key) const
        {
            const BucketType& bucket = m_Buckets[getBucketIndex(key)];
            for (const PairType& pair : bucket)
                if (pair.key == key)
                    return &pair;
            return nullptr;
        }

        bool operator==(const HashMap& other) const
        {
            if (count() != other.count())
                return false;

            for (const auto& [key, value] : *this)
            {
                const auto* pair = other.find(key);
                if (!pair) return false;
                if (pair->value != value) return false;
            }
            return true;
        }

        void clear()
        {
            for (auto& bucket : m_Buckets)
                bucket.clear();
            m_Count = 0;
        }

        bool isEmpty() const { return m_Count <= 0; }
    private:
        SizeType getBucketIndex(const Key& key) const
        {
            return m_Hasher(key) % m_Buckets.count();
        }

        Value& getOrInsert(const Key& key)
        {
            if (PairType* found = find(key))
                return found->value;

            BucketType& bucket = m_Buckets[getBucketIndex(key)];
            bucket.emplace(PairType{ key, Value{} });
            ++m_Count;
            return bucket[bucket.count() - 1].value;
        }

        Value& getOrInsert(Key&& key)
        {
            if (PairType* found = find(key))
                return found->value;

            BucketType& bucket = m_Buckets[getBucketIndex(key)];
            bucket.emplace(PairType{ std::move(key), Value{} });
            ++m_Count;
            return bucket[bucket.count() - 1].value;
        }
    private:
        Array<BucketType> m_Buckets;
        HasherType m_Hasher;
        SizeType m_Count = 0;
    };
}
