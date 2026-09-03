#pragma once
#include "Luma/Containers/StringView.h"
#include "Luma/Rendering/Texture.h"

namespace Luma
{
    struct IGpuDevice;
}

namespace Luma::TextureUtils
{
    ITexture* loadTexture(IGpuDevice* device, FStringView filepath);
    ITexture* loadTexture(IGpuDevice* device, const void* data, uint64_t dataSize);
    bool uploadTextureData(IGpuDevice* device, ITexture* texture, uint32_t arrayIndex, uint32_t mipLevel,
                           const void* data, size_t dataSize);
}
