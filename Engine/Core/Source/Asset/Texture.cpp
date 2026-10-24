#include "Luma/Asset/Texture.h"
#include "Luma/Rendering/TextureUtils.h"

namespace Luma
{
    uint32_t Texture::getWidth() const
    {
        return m_Texture ? m_Texture->getWidth() : 0;
    }

    uint32_t Texture::getHeight() const
    {
        return m_Texture ? m_Texture->getHeight() : 0;
    }

    uint32_t Texture::getDepth() const
    {
        return m_Texture ? m_Texture->getDepth() : 0;
    }

    Ref<Asset> PNGImporter::loadAsset(ArrayView<uint8_t> data) const
    {
        Ref<Texture> textureAsset = Ref<Texture>::create();
        textureAsset->m_Texture = TextureUtils::loadTexture(nullptr, data.data(), data.size());
        return textureAsset;
    }

    Ref<Asset> JPGImporter::loadAsset(ArrayView<uint8_t> data) const
    {
        Ref<Texture> textureAsset = Ref<Texture>::create();
        textureAsset->m_Texture = TextureUtils::loadTexture(nullptr, data.data(), data.size());
        return textureAsset;
    }

    Ref<Asset> TargaImporter::loadAsset(ArrayView<uint8_t> data) const
    {
        Ref<Texture> textureAsset = Ref<Texture>::create();
        textureAsset->m_Texture = TextureUtils::loadTexture(nullptr, data.data(), data.size());
        return textureAsset;
    }
}
