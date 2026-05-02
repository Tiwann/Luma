#pragma once
#include "Luma/Containers/StringView.h"
#include "Luma/Rendering/Texture.h"


namespace Luma::TextureUtils
{
    ITexture* loadTexture(IRenderDevice* device, FStringView filepath);
    ITexture* loadTexture(IRenderDevice* device, const void* data, uint64_t dataSize);
    bool uploadTextureData(IRenderDevice* device, ITexture* texture, uint32_t arrayIndex, uint32_t mipLevel,
                           const void* data, size_t dataSize);
}
