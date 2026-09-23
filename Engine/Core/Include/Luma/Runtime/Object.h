#pragma once
#include "Luma/Memory/RefCounted.h"

namespace Luma
{
    struct Object : RefCounted<Object>
    {
        ~Object() override = default;
        virtual void destroy(){}
    };
}