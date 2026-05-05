#include "Luma/Components/CameraComponent.h"
#include "Luma/Components/TransformComponent.h"
#include "Luma/Runtime/Entity.h"

namespace Luma
{
    void FCameraComponent::onUpdate(double deltaTime)
    {
        FTransformComponent* transform = getOwner()->getComponent<FTransformComponent>();
        if (!transform) return;

    }
}
