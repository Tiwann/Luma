#pragma once
#include "Luma/Runtime/Flags.h"

namespace Luma
{
    enum class ShaderStage
    {
        None = 0,
        Vertex = BIT(0),
        Geometry = BIT(1),
        Fragment = BIT(2),
        TessellationControl = BIT(3),
        TessellationEvaluation = BIT(4),
        Task = BIT(5),
        Mesh = BIT(6),

        Compute = BIT(7),

        AllGraphics = Vertex | Geometry | Fragment | TessellationControl | TessellationEvaluation | Task | Mesh
    };

    typedef Flags<ShaderStage> ShaderStageFlags;
}
