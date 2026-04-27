#pragma once
#include "Pair.h"
#include "Array.h"
#include "Luma/Runtime/Assertion.h"
#include <cstdint>


namespace Luma
{
    template<typename Key>
    struct THasher
    {
        uint64_t operator()(const Key& key) const
        {
            const uint8_t* data = reinterpret_cast<const uint8_t*>(&key);
            uint64_t hash = 14695981039346656037ULL;
            for (uint64_t i = 0; i < sizeof(Key); ++i)
            {
                hash ^= static_cast<uint64_t>(data[i]);
                hash *= 1099511628211ULL;
            }
            return hash;
        }
    };

    template<typename Key, typename Value, typename Hasher = THasher<Key>>
    class THashMap final
    {
    public:
        using SizeType = uint64_t;
        using PairType = TPair<Key, Value>;
        using BucketType = TArray<PairType>;

        struct Iterator
        {
            TArray<BucketType>* buckets = nullptr;
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
            const TArray<BucketType>* buckets = nullptr;
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

        THashMap()
        {
            for (SizeType i = 0; i < 16; ++i)
                m_Buckets.emplace(BucketType{});
        }

        explicit THashMap(SizeType bucketCount)
        {
            for (SizeType i = 0; i < bucketCount; ++i)
                m_Buckets.emplace(BucketType{});
        }

        Value& operator[](const Key& key) { return getOrInsert(key); }
        Value& operator[](Key&& key) { return getOrInsert(std::move(key)); }

        Value& at(const Key& key)
        {
            PairType* found = find(key);
            LUMA_ASSERT(found, "THashMap::at — key not found");
            return found->value;
        }

        const Value& at(const Key& key) const
        {
            const PairType* found = find(key);
            LUMA_ASSERT(found, "THashMap::at — key not found");
            return found->value;
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
        TArray<BucketType> m_Buckets;
        Hasher m_Hasher;
        SizeType m_Count = 0;
    };
}
