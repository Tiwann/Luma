#include "Luma/Components/TransformComponent.h"
#include "Luma/Runtime/Entity.h"

namespace Luma
{
    const FMatrix4f& FTransformComponent::getWorldSpaceMatrix()
    {
        const auto computeWorldSpaceMatrix = [&] -> FMatrix4f
        {
            const FMatrix4f& localMatrix = getLocalSpaceMatrix();
            const FEntity* owner = getOwner();
            if(const FEntity* parent = owner->getParent())
            {
                FTransformComponent* parentTransform = parent->getComponent<FTransformComponent>();
                if (!parentTransform) return localMatrix;
                const FMatrix4f& parentWorldMatrix = parentTransform->getWorldSpaceMatrix();
                return parentWorldMatrix * localMatrix;
            }
            return localMatrix;
        };

        return m_WorldSpaceMatrix.get(computeWorldSpaceMatrix);
    }

    const FMatrix4f& FTransformComponent::getLocalSpaceMatrix()
    {
        return getMatrix();
    }
}
