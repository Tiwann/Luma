#pragma once
#include "ResourceState.h"
#include "ResourceType.h"

namespace Luma
{
    struct IResource
    {
        virtual ~IResource() = default;
        virtual EResourceType getResourceType() const = 0;
        virtual EResourceState getResourceState() const = 0;
    };
}
