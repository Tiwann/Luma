#pragma once
#include "Luma/Containers/StringView.h"
#include "Luma/Rendering/Texture.h"


using Luma::RHI::Device;
using Luma::RHI::Texture;

namespace Luma::TextureUtils
{
    Texture* loadTexture(Device* device, StringView filepath);
    Texture* loadTexture(Device* device, const void* data, uint64_t dataSize);
    bool uploadTextureDataSync(Device* device, Texture* texture, uint32_t arrayIndex, uint32_t mipLevel,
                           const void* data, size_t dataSize);
    bool generateMips(Device* device, Texture* texture, uint32_t sourceMip, uint32_t destMip, uint32_t mipCount);
}
