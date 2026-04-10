#pragma once

namespace luma
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
