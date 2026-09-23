#include "Luma/Rendering/ResourceBarrier.h"

namespace Luma
{
    ResourceAccessFlags getSourceAccessFlags(const ResourceState resourceState)
    {
        switch (resourceState)
        {
        case ResourceState::Undefined: return ResourceAccess::None;
        case ResourceState::ColorTarget: return ResourceAccess::ColorTargetWrite;
        case ResourceState::DepthStencilTarget: return ResourceAccess::DepthStencilTargetWrite;
        case ResourceState::CopySource: return ResourceAccess::CopyRead;
        case ResourceState::CopyDest: return ResourceAccess::CopyWrite;
        case ResourceState::ShaderRead: return ResourceAccess::ShaderRead;
        case ResourceState::ShaderWrite: return ResourceAccess::ShaderWrite;
        case ResourceState::General: return ResourceAccess::None;
        default: return ResourceAccess::None;
        }
    }

    ResourceAccessFlags getDestAccessFlags(const ResourceState resourceState)
    {
        switch (resourceState)
        {
        case ResourceState::Undefined: return ResourceAccess::None;
        case ResourceState::ColorTarget: return ResourceAccess::ColorTargetRead;
        case ResourceState::DepthStencilTarget: return ResourceAccess::DepthStencilTargetRead;
        case ResourceState::CopySource: return ResourceAccess::CopyRead;
        case ResourceState::CopyDest: return ResourceAccess::CopyWrite;
        case ResourceState::ShaderRead: return ResourceAccess::ShaderRead;
        case ResourceState::ShaderWrite: return ResourceAccess::ShaderWrite;
        case ResourceState::General: return ResourceAccess::None;
        default: return ResourceAccess::None;
        }
    }
}
