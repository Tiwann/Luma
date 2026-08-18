#pragma once
#include <box3d/box3d.h>

namespace Luma
{
    struct FPhysicsWorld::Impl
    {
        b3WorldId worldId{b3_nullWorldId};
        float timestep = 1.0f / 60.0f;
        uint32_t substepCount = 4;
    };
}