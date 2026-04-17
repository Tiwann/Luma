#pragma once
#include "ResourceState.h"
#include "ResourceType.h"

namespace Luma
{
    struct IResource
    {
        virtual ~IResource() = default;
        virtual EResourceType getResourceType() = 0;
        virtual EResourceState getResourceState() = 0;
    };
}
