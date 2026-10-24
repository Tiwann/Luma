#pragma once
#include "Luma/Memory/RefCounted.h"
#include "Luma/Runtime/RTTI.h"

namespace Luma
{
    struct Object : RefCounted<Object>
    {
        LUMA_CLASS_ROOT(Object)
    public:
        ~Object() override = default;
        virtual void destroy(){}
    };
}