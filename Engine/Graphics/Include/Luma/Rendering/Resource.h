#pragma once
#include "Interface.h"
#include "ResourceState.h"
#include "ResourceType.h"

namespace luma
{
    struct IResource : IBase
    {
        ~IResource() override = default;
        virtual EResourceType getResourceType() = 0;
        virtual void setResourceState(EResourceState state) = 0;
        virtual EResourceState getResourceState() = 0;
    };
}
