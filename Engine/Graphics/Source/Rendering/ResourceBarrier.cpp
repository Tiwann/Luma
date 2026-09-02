#include "Luma/Rendering/ResourceBarrier.h"

namespace Luma
{
    FResourceAccessFlags getSourceAccessFlags(const EResourceState resourceState)
    {
        switch (resourceState)
        {
        case EResourceState::Undefined: return EResourceAccessBits::None;
        case EResourceState::ColorTarget: return EResourceAccessBits::ColorTargetWrite;
        case EResourceState::DepthStencilTarget: return EResourceAccessBits::DepthStencilTargetWrite;
        case EResourceState::CopySource: return EResourceAccessBits::CopyRead;
        case EResourceState::CopyDest: return EResourceAccessBits::CopyWrite;
        case EResourceState::ShaderRead: return EResourceAccessBits::ShaderRead;
        case EResourceState::ShaderWrite: return EResourceAccessBits::ShaderWrite;
        case EResourceState::General: return EResourceAccessBits::None;
        default: return EResourceAccessBits::None;
        }
    }

    FResourceAccessFlags getDestAccessFlags(const EResourceState resourceState)
    {
        switch (resourceState)
        {
        case EResourceState::Undefined: return EResourceAccessBits::None;
        case EResourceState::ColorTarget: return EResourceAccessBits::ColorTargetRead;
        case EResourceState::DepthStencilTarget: return EResourceAccessBits::DepthStencilTargetRead;
        case EResourceState::CopySource: return EResourceAccessBits::CopyRead;
        case EResourceState::CopyDest: return EResourceAccessBits::CopyWrite;
        case EResourceState::ShaderRead: return EResourceAccessBits::ShaderRead;
        case EResourceState::ShaderWrite: return EResourceAccessBits::ShaderWrite;
        case EResourceState::General: return EResourceAccessBits::None;
        default: return EResourceAccessBits::None;
        }
    }
}
