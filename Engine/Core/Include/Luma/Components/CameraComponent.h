#pragma once
#include "Luma/Rendering/Camera.h"
#include "Luma/Runtime/Component.h"

namespace Luma
{
    class FCameraComponent : public IComponent, public TCamera<float>
    {

    };
}

