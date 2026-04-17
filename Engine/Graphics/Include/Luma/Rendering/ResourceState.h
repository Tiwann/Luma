#pragma once

namespace Luma
{
    enum class EResourceState
    {
        Undefined,
        General,
        ShaderRead,
        ShaderWrite,
        ColorAttachment,
        DepthStencilAttachment,
        CopySource,
        CopyDest,
        Present,
    };
}
