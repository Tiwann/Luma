#include "Luma/Components/TransformComponent.h"
#include "Luma/Runtime/Entity.h"

namespace Luma
{
    const FMatrix4f& TransformComponent::getWorldSpaceMatrix()
    {
        const auto computeWorldSpaceMatrix = [&] -> FMatrix4f
        {
            const FMatrix4f& localMatrix = getLocalSpaceMatrix();
            const Entity* owner = getOwner();
            if(const Entity* parent = owner->getParent())
            {
                TransformComponent* parentTransform = parent->getComponent<TransformComponent>();
                if (!parentTransform) return localMatrix;
                const FMatrix4f& parentWorldMatrix = parentTransform->getWorldSpaceMatrix();
                return parentWorldMatrix * localMatrix;
            }
            return localMatrix;
        };

        return m_WorldSpaceMatrix.get(computeWorldSpaceMatrix);
    }

    const FMatrix4f& TransformComponent::getLocalSpaceMatrix()
    {
        return getMatrix();
    }
}
