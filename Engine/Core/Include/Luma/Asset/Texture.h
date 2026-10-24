#pragma once
#include "AssetImporter.h"
#include "Luma/Rendering/Texture.h"
#include "Luma/Runtime/Asset.h"


namespace Luma
{
    class Texture : public Asset
    {
        LUMA_CLASS_INLINE(Texture, Asset)
    public:
        AssetType getAssetType() const override { return AssetType::Texture; }

        uint32_t getWidth() const;
        uint32_t getHeight() const;

    private:
        friend struct AssetImporter;

        Ref<RHI::Texture> m_Texture = nullptr;
    };

    class EnvironmentTexture : public Asset
    {
        LUMA_CLASS_INLINE(EnvironmentTexture, Asset)
    public:
        enum class Type
        {
            None,
            SixSided,
            Equirect
        };

        Type getType() const { return m_Type; }
        uint32_t getWidth() const;
        uint32_t getHeight() const;

    private:
        Type m_Type = Type::None;
        Ref<RHI::Texture> m_Texture = nullptr;
    };

    class PNGImporter final : public AssetImporter
    {
    public:
        Array<StringView> getSupportedExtensions() const override { return {".png"}; }
        AssetType getAssetType() const override { return AssetType::Texture; }
        Ref<Asset> loadAsset(ArrayView<uint8_t> data) const override;
    };

    class JPGImporter final : public AssetImporter
    {
    public:
        Array<StringView> getSupportedExtensions() const override { return {".jpg", ".jpeg"}; }
        AssetType getAssetType() const override { return AssetType::Texture; }
        Ref<Asset> loadAsset(ArrayView<::uint8_t> data) const override;
    };

    class TargaImporter final : public AssetImporter
    {
    public:
        Array<StringView> getSupportedExtensions() const override { return {".tga"}; }
        AssetType getAssetType() const override { return AssetType::Texture; }
        Ref<Asset> loadAsset(ArrayView<::uint8_t> data) const override;
    };
}
