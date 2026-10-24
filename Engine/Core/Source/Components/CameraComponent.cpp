#include "Luma/Components/CameraComponent.h"
#include "Luma/Components/TransformComponent.h"
#include "Luma/Runtime/Entity.h"

LUMA_DEFINE_CLASS(Luma::CameraComponent)

namespace Luma
{
    void CameraComponent::onUpdate(double deltaTime)
    {
        TransformComponent* transform = getOwner()->getComponent<TransformComponent>();
        if (!transform) return;


    }
}
