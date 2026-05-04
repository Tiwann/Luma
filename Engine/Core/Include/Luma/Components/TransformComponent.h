#pragma once
#include "Luma/Runtime/Component.h"
#include "Luma/Math/Transform.h"

namespace Luma
{
    class FTransformComponent : public IComponent, public TTransform<float>
    {
    public:
        FTransformComponent() = default;

        const FMatrix4f& getWorldSpaceMatrix();
        const FMatrix4f& getLocalSpaceMatrix();
    private:
        TLazy<FMatrix4f> m_WorldSpaceMatrix;
    };
}
