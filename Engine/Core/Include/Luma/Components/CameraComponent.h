#pragma once
#include "Luma/Rendering/Camera.h"
#include "Luma/Runtime/Component.h"

namespace Luma
{
    class CameraComponent : public Component, public TCamera<float>
    {
        LUMA_CLASS(CameraComponent, Component)

        void onUpdate(double deltaTime) override;
    };
}

