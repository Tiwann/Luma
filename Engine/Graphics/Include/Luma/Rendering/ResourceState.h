#pragma once

namespace Luma
{
    enum class ResourceState
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
