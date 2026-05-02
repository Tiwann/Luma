#pragma once
#include "Luma/Runtime/Component.h"
#include "Luma/Math/Transform.h"

namespace Luma
{
    class FTransformComponent : public IComponent, public TTransform<float>
    {

    };
}
