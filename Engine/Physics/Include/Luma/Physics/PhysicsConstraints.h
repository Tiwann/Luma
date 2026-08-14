#pragma once
#include "Luma/Runtime/Flags.h"

namespace Luma
{
    enum class EPhysicsConstraintsBits
    {
        None = 0,
        PositionX = BIT(0),
        PositionY = BIT(1),
        PositionZ = BIT(2),
        RotationX = BIT(3),
        RotationY = BIT(4),
        RotationZ = BIT(5),

        Position = PositionX | PositionY | PositionZ,
        Rotation = RotationX | RotationY | RotationZ,
    };

    using FPhysicsConstraintsFlags = TFlags<EPhysicsConstraintsBits>;
}
