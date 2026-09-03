#pragma once
#include "Luma/Containers/BufferView.h"
#include "Luma/Containers/StringView.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Runtime/Asset.h"

namespace msdfgen { class FontHandle; }

namespace Luma
{
    struct ITexture;
    struct IGpuDevice;

    enum class EFontAtlasType
    {
        None,
        Bitmap,
        SDF,
        MSDF
    };

    struct FFontMetrics
    {
        double emSize = 0.0;
        double ascenderY = 0.0;
        double descenderY = 0.0;
        double lineHeight = 0.0;
        double underlineY = 0.0;
        double underlineThickness = 0.0;
    };

    struct FCharacterSet
    {
        uint32_t begin = 0;
        uint32_t end = 0;

        static constexpr const FCharacterSet& ascii() { static constexpr FCharacterSet ascii{0x20, 0x7f}; return ascii;}
    };

    class FFont : public IAsset
    {
    public:
        FFont();
        ~FFont() override;

        static EAssetType getStaticAssetType() { return EAssetType::Font; }
        EAssetType getAssetType() const override { return getStaticAssetType(); }

        bool loadAndGenerate(const TBufferView<uint8_t>& fontData, EFontAtlasType atlasType, const TArray<FCharacterSet>& charSets, IGpuDevice* device);
        bool loadAndGenerate(FStringView filepath, EFontAtlasType atlasType, const TArray<FCharacterSet>& charSets, IGpuDevice* device);
        bool loadFromAtlas(FStringView atlasFilepath, FStringView fontDataFilepath);
        void destroy() override;

        EFontAtlasType getAtlasType() const;
        Ref<ITexture> getAtlasTexture() const;
        msdfgen::FontHandle* getHandle() const;

        double getAdvance(uint32_t character) const;
        double getAdvance(uint32_t character, uint32_t nextCharacter) const;
        bool hasGlyph(uint32_t unicode) const;

        FFontMetrics getMetrics() const;
        void getAtlasTextureCoordinates(uint32_t unicode, double& left, double& right, double& top, double& bottom) const;
        void getPlaneBounds(uint32_t unicode, double& left, double& right, double& top, double& bottom) const;

        double getTextWidth(FStringView text, float fontSize, float characterSpacing = 1.0f) const;
        double getTextHeight(FStringView text, float fontSize, float lineSpacing = 1.0f) const;
    private:
        EFontAtlasType m_AtlasType = EFontAtlasType::None;
        Ref<ITexture> m_AtlasTexture = nullptr;
        struct FPrivate;
        FPrivate* m_PrivateData = nullptr;
    };

    enum class EFontStyle
    {
        Regular,
        Italic,
        Bold,
        BoldItalic,
        Light,
        LightItalic,
        Medium,
        SemiBold,
        Black,
        Condensed,
        CondensedBold,
        Extended
    };

    struct FFontFamily
    {
        FString name = "Unnamed Font";
        Ref<FFont> regular = nullptr;
        Ref<FFont> italic = nullptr;
        Ref<FFont> bold = nullptr;
        Ref<FFont> boldItalic = nullptr;
        Ref<FFont> light = nullptr;
        Ref<FFont> lightItalic = nullptr;
        Ref<FFont> medium = nullptr;
        Ref<FFont> semiBold = nullptr;
        Ref<FFont> black = nullptr;
        Ref<FFont> condensed = nullptr;
        Ref<FFont> condensedBold = nullptr;
        Ref<FFont> extended = nullptr;
    };
}
