#pragma once
#include "Luma/Memory/Ref.h"
#include "Luma/Rendering/GPUDevice.h"
#include "Luma/Rendering/Shader.h"
#include "Luma/Rendering/Texture.h"

namespace Luma
{
    static bool fullScreenBlit(Ref<IGPUDevice> device, Ref<ITexture> source, Ref<ITexture> dest)
    {
        if (!device) return false;
        if (!source) return false;
        if (!dest) return false;



    }
}