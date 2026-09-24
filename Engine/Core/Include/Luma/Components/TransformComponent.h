#pragma once
#include "Luma/Runtime/Component.h"
#include "Luma/Math/Transform.h"

namespace Luma
{
    class TransformComponent : public Component, public TTransform<float>
    {
    public:
        TransformComponent() = default;

        const FMatrix4f& getWorldSpaceMatrix();
        const FMatrix4f& getLocalSpaceMatrix();
    private:
        Lazy<FMatrix4f> m_WorldSpaceMatrix;
    };
}
