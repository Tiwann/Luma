#pragma once
#include "Luma/Math/Vector2.h"
#include "Luma/Math/Vector3.h"
#include "Luma/Math/Vector4.h"

namespace Luma
{
    struct MeshVertex
    {
        FVector3f position = FVector3f::Zero;
        FVector2f texCoords = FVector2f::Zero;
        FVector3f normal = FVector3f::Zero;
        FVector3f tangent = FVector3f::Zero;
        FVector4f color = FVector4f::Zero;
    };
}