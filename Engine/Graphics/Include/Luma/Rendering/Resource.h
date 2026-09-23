#pragma once
#include "ResourceState.h"
#include "ResourceType.h"

namespace Luma::RHI
{
    struct Resource
    {
        virtual ~Resource() = default;
        virtual ResourceType getResourceType() const = 0;
        virtual ResourceState getResourceState() const = 0;
    };
}
