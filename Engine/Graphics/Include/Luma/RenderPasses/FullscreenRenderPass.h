#pragma once
#include "Luma/Rendering/RenderPass.h"


namespace Luma
{
    class FullscreenRenderPass : RenderPass
    {
    public:
        void setup() override;

        void execute(Ref<CommandBuffer> cmdBuf) override;

    private:
    };
}
