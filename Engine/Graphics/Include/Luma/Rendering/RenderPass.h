#pragma once
#include "Luma/Memory/Ref.h"
#include "Luma/Rendering/Forward.h"

namespace Luma
{
    struct RenderPass
    {
        virtual ~RenderPass() = default;
        virtual void setup() = 0;
        virtual void execute(Ref<CommandBuffer> cmdBuf) = 0;

    };
}
