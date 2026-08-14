#pragma once

namespace Luma
{
    struct FPhysicsMaterial
    {
        float density = 1.0f;
        float friction = 0.6f;
        float restitution = 0.0f;

        constexpr FPhysicsMaterial() = default;
    };
}
