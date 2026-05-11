#pragma once
#include "Entity.h"
#include <cstdint>

namespace Luma
{
    class FEntityHandle
    {
    public:
        FEntityHandle(uint32_t handle, FScene* context);
        FEntityHandle(decltype(nullptr));

        FEntity* operator->();

        uint32_t getHandle() const { return m_Handle; }
        FScene* getContext() const { return m_Context; }
    private:
        uint32_t m_Handle = 0;
        FScene* m_Context = nullptr;
    };
}
