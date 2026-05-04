#pragma once
#include "Luma/Memory/RefCounted.h"

namespace Luma
{
    struct IObject : IRefCounted<IObject>
    {
        ~IObject() override = default;
        virtual void destroy(){}
    };
}