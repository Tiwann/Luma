#include "Luma/Rendering/Viewport.h"

namespace Luma
{
    Viewport::Viewport(const FRect2f& bounds, float minDepth, float maxDepth)
        : x(bounds.x), y(bounds.y), minDepth(minDepth), maxDepth(maxDepth){}
}
