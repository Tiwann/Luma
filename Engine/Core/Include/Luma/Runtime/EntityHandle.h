#pragma once
#include "Entity.h"
#include <cstdint>

namespace Luma
{
    class FEntityHandle
    {
    public:
        FEntityHandle(uint32_t handle, Scene* context);
        FEntityHandle(decltype(nullptr));

        Entity* operator->();

        uint32_t getHandle() const { return m_Handle; }
        Scene* getContext() const { return m_Context; }
    private:
        uint32_t m_Handle = 0;
        Scene* m_Context = nullptr;
    };
}
