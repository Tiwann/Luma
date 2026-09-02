#pragma once

namespace Luma
{
    enum class EResourceState
    {
        Undefined,
        General,
        ShaderRead,
        ShaderWrite,
        ColorTarget,
        DepthStencilTarget,
        CopySource,
        CopyDest,
        Present,
    };
}
