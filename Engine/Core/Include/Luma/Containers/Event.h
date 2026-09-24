#pragma once
#include "Array.h"
#include "Luma/Runtime/Assertion.h"

#include <functional>
#include <algorithm>
#include <cstdint>

namespace Luma
{
    template <typename Signature>
    class Event;

    template <typename R, typename... Args>
    class Event<R(Args...)>
    {
    public:
        using FunctionType = std::function<R(Args...)>;
        using ReturnType = R;

        Event() = default;
        ~Event() = default;
        Event(const Event&) = default;
        Event& operator=(const Event&) = default;
        Event(Event&&) = default;
        Event& operator=(Event&&) = default;

        template <typename Callable>
        uint64_t bind(Callable&& callable)
        {
            return bind(FunctionType(std::forward<Callable>(callable)));
        }

        template <typename T>
        uint64_t bindMember(T* object, R(T::*memberFn)(Args...))
        {
            return bind([object, memberFn](Args... args) -> R
            {
                return (object->*memberFn)(std::forward<Args>(args)...);
            });
        }

        template <typename T>
        uint64_t bindMemberConst(const T* object, R(T::*memberFn)(Args...) const)
        {
            return bind([object, memberFn](Args... args) -> R
            {
                return (object->*memberFn)(std::forward<Args>(args)...);
            });
        }


        bool unbind(uint64_t handle)
        {
            if (handle == 0) return false;
            const Entry* found = m_Entries.single([&handle](const Entry& entry) -> bool { return entry.handle == handle; });
            if (!found) return false;
            m_Entries.remove(*found);
            return true;
        }

        void unbindAll()
        {
            m_Entries.clear();
        }

        bool isBound() const noexcept { return !m_Entries.empty(); }

        bool isHandleBound(uint64_t handle) const noexcept
        {
            return m_Entries.any([&handle](const Entry& entry) { return entry.handle == handle; });
        }

        size_t count() const noexcept { return m_Entries.size(); }

        auto broadcast(Args... args) const
        {
            if constexpr (std::is_void_v<R>)
            {
                for (const auto& entry : m_Entries)
                    entry.fn(std::forward<Args>(args)...);
            }
            else
            {
                Array<R> results;
                for (const auto& entry : m_Entries)
                    results.add(entry.fn(std::forward<Args>(args)...));
                return results;
            }
        }
        
        auto operator()(Args... args) const
        {
            return broadcast(std::forward<Args>(args)...);
        }

    private:
        struct Entry
        {
            uint64_t handle;
            FunctionType fn;
        };

        uint64_t next() noexcept
        {
            return ++m_NextHandle;
        }

        uint64_t bind(FunctionType fn)
        {
            LUMA_ASSERT(fn, "TEvent — cannot bind an empty callable.");
            const uint64_t handle = next();
            m_Entries.emplace(Entry(handle, std::move(fn)));
            return handle;
        }

        Array<Entry> m_Entries;
        uint64_t m_NextHandle{0};
    };
}
