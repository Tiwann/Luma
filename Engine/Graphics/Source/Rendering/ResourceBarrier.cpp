#include "Luma/Rendering/ResourceBarrier.h"

namespace Luma
{
    FResourceAccessFlags getSourceAccessFlags(const EResourceState resourceState)
    {
        switch (resourceState)
        {
        case EResourceState::Undefined: return EResourceAccessBits::None;
        case EResourceState::ColorAttachment: return EResourceAccessBits::ColorAttachmentWrite;
        case EResourceState::DepthStencilAttachment: return EResourceAccessBits::DepthStencilAttachmentWrite;
        case EResourceState::CopySource: return EResourceAccessBits::TransferRead;
        case EResourceState::CopyDest: return EResourceAccessBits::TransferWrite;
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
        case EResourceState::ColorAttachment: return EResourceAccessBits::ColorAttachmentRead;
        case EResourceState::DepthStencilAttachment: return EResourceAccessBits::DepthStencilAttachmentRead;
        case EResourceState::CopySource: return EResourceAccessBits::TransferRead;
        case EResourceState::CopyDest: return EResourceAccessBits::TransferWrite;
        case EResourceState::ShaderRead: return EResourceAccessBits::ShaderRead;
        case EResourceState::ShaderWrite: return EResourceAccessBits::ShaderWrite;
        case EResourceState::General: return EResourceAccessBits::None;
        default: return EResourceAccessBits::None;
        }
    }
}
