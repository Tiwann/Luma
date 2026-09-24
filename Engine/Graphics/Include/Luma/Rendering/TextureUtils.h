#pragma once
#include "Luma/Containers/StringView.h"
#include "Luma/Rendering/Texture.h"

namespace Luma::RHI
{
    struct Device;
    struct Texture;
}

namespace Luma::TextureUtils
{
    RHI::Texture* loadTexture(RHI::Device* device, StringView filepath);
    RHI::Texture* loadTexture(RHI::Device* device, const void* data, uint64_t dataSize);
    bool uploadTextureDataSync(RHI::Device* device, RHI::Texture* texture, uint32_t arrayIndex, uint32_t mipLevel,
                           const void* data, size_t dataSize);
}
