#pragma once
#include "Luma/Containers/StringView.h"
#include "Luma/Rendering/Texture.h"

namespace Luma
{
    struct IGPUDevice;
}

namespace Luma::TextureUtils
{
    ITexture* loadTexture(IGPUDevice* device, FStringView filepath);
    ITexture* loadTexture(IGPUDevice* device, const void* data, uint64_t dataSize);
    bool uploadTextureDataSync(IGPUDevice* device, ITexture* texture, uint32_t arrayIndex, uint32_t mipLevel,
                           const void* data, size_t dataSize);
}
