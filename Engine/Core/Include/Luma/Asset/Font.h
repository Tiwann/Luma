#pragma once
#include "Luma/Containers/BufferView.h"
#include "Luma/Containers/StringView.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Runtime/Asset.h"

namespace msdfgen { class FontHandle; }

namespace Luma
{
    namespace RHI
    {
        struct Device;
        struct Texture;
    }


    enum class FontAtlasType
    {
        None,
        Bitmap,
        SDF,
        MSDF
    };

    struct FontMetrics
    {
        double emSize = 0.0;
        double ascenderY = 0.0;
        double descenderY = 0.0;
        double lineHeight = 0.0;
        double underlineY = 0.0;
        double underlineThickness = 0.0;
    };

    struct CharacterSet
    {
        uint32_t begin = 0;
        uint32_t end = 0;

        static constexpr const CharacterSet& ascii() { static constexpr CharacterSet ascii{0x20, 0x7f}; return ascii;}
    };

    class Font : public Asset
    {
    public:
        Font();
        ~Font() override;

        static AssetType getStaticAssetType() { return AssetType::Font; }
        AssetType getAssetType() const override { return getStaticAssetType(); }

        bool loadAndGenerate(const BufferView<uint8_t>& fontData, FontAtlasType atlasType, const Array<CharacterSet>& charSets, RHI::Device* device);
        bool loadAndGenerate(StringView filepath, FontAtlasType atlasType, const Array<CharacterSet>& charSets, RHI::Device* device);
        bool loadFromAtlas(StringView atlasFilepath, StringView fontDataFilepath);
        void destroy() override;

        FontAtlasType getAtlasType() const;
        Ref<RHI::Texture> getAtlasTexture() const;
        msdfgen::FontHandle* getHandle() const;

        double getAdvance(uint32_t character) const;
        double getAdvance(uint32_t character, uint32_t nextCharacter) const;
        bool hasGlyph(uint32_t unicode) const;

        FontMetrics getMetrics() const;
        void getAtlasTextureCoordinates(uint32_t unicode, double& left, double& right, double& top, double& bottom) const;
        void getPlaneBounds(uint32_t unicode, double& left, double& right, double& top, double& bottom) const;

        double getTextWidth(StringView text, float fontSize, float characterSpacing = 1.0f) const;
        double getTextHeight(StringView text, float fontSize, float lineSpacing = 1.0f) const;
    private:
        FontAtlasType m_AtlasType = FontAtlasType::None;
        Ref<RHI::Texture> m_AtlasTexture = nullptr;
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
        String name = "Unnamed Font";
        Ref<Font> regular = nullptr;
        Ref<Font> italic = nullptr;
        Ref<Font> bold = nullptr;
        Ref<Font> boldItalic = nullptr;
        Ref<Font> light = nullptr;
        Ref<Font> lightItalic = nullptr;
        Ref<Font> medium = nullptr;
        Ref<Font> semiBold = nullptr;
        Ref<Font> black = nullptr;
        Ref<Font> condensed = nullptr;
        Ref<Font> condensedBold = nullptr;
        Ref<Font> extended = nullptr;
    };
}
