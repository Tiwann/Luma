#include "Luma/Components/CameraComponent.h"
#include "Luma/Components/TransformComponent.h"
#include "Luma/Runtime/Entity.h"

namespace Luma
{
    void CameraComponent::onUpdate(double deltaTime)
    {
        TransformComponent* transform = getOwner()->getComponent<TransformComponent>();
        if (!transform) return;


    }
}
