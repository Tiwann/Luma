#pragma once
#include "Luma/Containers/StringView.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Runtime/Asset.h"

namespace msdfgen { class FontHandle; }

namespace Luma
{
    struct ITexture;
    struct IRenderDevice;

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

        bool loadAndGenerate(FStringView filepath, EFontAtlasType atlasType, const TArray<FCharacterSet>& charSets, IRenderDevice* device);
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

    struct FFontFamilyDesc
    {
        FString name = "Unnamed Font";
        FFont* regular = nullptr;
        FFont* italic = nullptr;
        FFont* bold = nullptr;
        FFont* boldItalic = nullptr;
        FFont* light = nullptr;
        FFont* lightItalic = nullptr;
        FFont* medium = nullptr;
        FFont* semiBold = nullptr;
        FFont* black = nullptr;
        FFont* condensed = nullptr;
        FFont* condensedBold = nullptr;
        FFont* extended = nullptr;
    };

    class FFontFamily
    {
    public:
        FFontFamily(FFont* font) : m_Regular(font) {}
        FFontFamily(const FFontFamilyDesc& desc) :
        m_Name(desc.name),
        m_Regular(desc.regular),
        m_Italic(desc.italic),
        m_Bold(desc.bold),
        m_BoldItalic(desc.boldItalic),
        m_Light(desc.light),
        m_LightItalic(desc.lightItalic),
        m_Medium(desc.medium),
        m_SemiBold(desc.semiBold),
        m_Black(desc.black),
        m_Condensed(desc.condensed),
        m_CondensedBold(desc.condensedBold),
        m_Extended(desc.extended){}

        bool hasRegular() const { return m_Regular; }
        bool hasItalic() const { return m_Italic; }
        bool hasBold() const { return m_Bold; }
        bool hasBoldItalic() const { return m_BoldItalic; }
        bool hasLight() const { return m_Light; }
        bool hasLightItalic() const { return m_LightItalic; }
        bool hasMedium() const { return m_Medium; }
        bool hasSemiBold() const { return m_SemiBold; }
        bool hasBlack() const { return m_Black; }
        bool hasCondensed() const { return m_Condensed; }
        bool hasCondensedBold() const { return m_CondensedBold; }
        bool hasExtended() const { return m_Extended; }

        Ref<FFont> getRegular() const { return m_Regular; }
        Ref<FFont> getItalic() const { return m_Italic; }
        Ref<FFont> getBold() const { return m_Bold; }
        Ref<FFont> getBoldItalic() const { return m_BoldItalic; }
        Ref<FFont> getLight() const { return m_Light; }
        Ref<FFont> getLightItalic() const { return m_LightItalic; }
        Ref<FFont> getMedium() const { return m_Medium; }
        Ref<FFont> getSemiBold() const { return m_SemiBold; }
        Ref<FFont> getBlack() const { return m_Black; }
        Ref<FFont> getCondensed() const { return m_Condensed; }
        Ref<FFont> getCondensedBold() const { return m_CondensedBold; }
        Ref<FFont> getExtended() const { return m_Extended; }

        Ref<FFont> getFont(EFontStyle style) const
        {
            switch (style)
            {
            case EFontStyle::Regular: return m_Regular;
            case EFontStyle::Italic: return m_Italic;
            case EFontStyle::Bold: return m_Bold;
            case EFontStyle::BoldItalic: return m_BoldItalic;
            case EFontStyle::Light: return m_Light;
            case EFontStyle::LightItalic: return m_LightItalic;
            case EFontStyle::Medium: return m_Medium;
            case EFontStyle::SemiBold: return m_SemiBold;
            case EFontStyle::Black: return m_Black;
            case EFontStyle::Condensed: return m_Condensed;
            case EFontStyle::CondensedBold: return m_CondensedBold;
            case EFontStyle::Extended: return m_Extended;
            default: return nullptr;
            }
        }
    private:
        FString m_Name;

        Ref<FFont> m_Regular = nullptr;
        Ref<FFont> m_Italic = nullptr;
        Ref<FFont> m_Bold = nullptr;
        Ref<FFont> m_BoldItalic = nullptr;
        Ref<FFont> m_Light = nullptr;
        Ref<FFont> m_LightItalic = nullptr;
        Ref<FFont> m_Medium = nullptr;
        Ref<FFont> m_SemiBold = nullptr;
        Ref<FFont> m_Black = nullptr;
        Ref<FFont> m_Condensed = nullptr;
        Ref<FFont> m_CondensedBold = nullptr;
        Ref<FFont> m_Extended = nullptr;
    };
}
