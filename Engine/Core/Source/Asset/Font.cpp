#include "Luma/Asset/Font.h"
#include "Luma/Runtime/Format.h"
#include "Luma/Rendering/Texture.h"
#include "Luma/Rendering/GpuDevice.h"
#include "Luma/Rendering/TextureUtils.h"
#include <msdf-atlas-gen/msdf-atlas-gen.h>

#include "Luma/Runtime/FileUtils.h"

namespace Luma
{
    static constexpr uint32_t ATLAS_DEFAULT_SIZE = 256;

    struct FFont::FPrivate
    {
        msdfgen::FontHandle* handle = nullptr;
        std::vector<msdf_atlas::GlyphGeometry> glyphs;
        msdf_atlas::FontGeometry fontGeometry;
    };

    FFont::FFont()
    {
        m_PrivateData = new FPrivate;
    }

    FFont::~FFont()
    {
        delete m_PrivateData;
    }

    EFontAtlasType FFont::getAtlasType() const
    {
        return m_AtlasType;
    }

    Ref<ITexture> FFont::getAtlasTexture() const
    {
        return m_AtlasTexture;
    }

    msdfgen::FontHandle* FFont::getHandle() const
    {
        return m_PrivateData->handle;
    }

    double FFont::getAdvance(uint32_t character) const
    {
        const auto* glyph = m_PrivateData->fontGeometry.getGlyph(character);
        if (!glyph) return 0.0;
        return glyph->getAdvance();
    }

    double FFont::getAdvance(const uint32_t character, const uint32_t nextCharacter) const
    {
        double advance = 0.0;
        m_PrivateData->fontGeometry.getAdvance(advance, character, nextCharacter);
        return advance;
    }

    bool FFont::hasGlyph(uint32_t unicode) const
    {
        return m_PrivateData->fontGeometry.getGlyph(unicode);
    }

    FFontMetrics FFont::getMetrics() const
    {
        const auto metrics = m_PrivateData->fontGeometry.getMetrics();

        FFontMetrics result;
        result.ascenderY = metrics.ascenderY;
        result.descenderY = metrics.descenderY;
        result.emSize = metrics.emSize;
        result.lineHeight = metrics.lineHeight;
        result.underlineThickness = metrics.underlineThickness;
        result.underlineY = metrics.underlineY;
        return result;
    }

    void FFont::getAtlasTextureCoordinates(const uint32_t unicode,
        double& left,
        double& right,
        double& top,
        double& bottom) const
    {
        const auto glyph = m_PrivateData->fontGeometry.getGlyph(unicode);
        glyph->getQuadAtlasBounds(left, bottom, right, top);
        left /= m_AtlasTexture->getWidth();
        right /= m_AtlasTexture->getWidth();
        bottom /= m_AtlasTexture->getHeight();
        top /= m_AtlasTexture->getHeight();
    }

    void FFont::getPlaneBounds(const uint32_t unicode,
        double& left,
        double& right,
        double& top,
        double& bottom) const
    {
        const auto glyph = m_PrivateData->fontGeometry.getGlyph(unicode);
        glyph->getQuadPlaneBounds(left, bottom, right, top);
    }

    double FFont::getTextHeight(FStringView text, float fontSize, float lineSpacing) const
    {
        const FFontMetrics metrics = getMetrics();
        const double fsScale = fontSize / (metrics.ascenderY - metrics.descenderY);

        double posY = fsScale * metrics.ascenderY;

        for (size_t index = 0; index < text.count(); index++)
        {
            const auto character = hasGlyph(text[index]) ? text[index] : '?';

            if (!hasGlyph(character))
                continue;

            if (character == L'\r')
                continue;

            if (character == L'\t')
                continue;

            if (character == L'\n')
            {
                posY += fsScale * metrics.lineHeight + lineSpacing;
                continue;
            }
        }

        return posY;
    }

    double FFont::getTextWidth(FStringView text, float fontSize, float characterSpacing) const
    {
        const FFontMetrics metrics = getMetrics();
        const double fsScale = fontSize / (metrics.ascenderY - metrics.descenderY);

        double posX = 0.0;

        for (size_t index = 0; index < text.count(); index++)
        {
            const auto character = hasGlyph(text[index]) ? text[index] : '?';

            if (!hasGlyph(character))
                continue;

            if (character == L'\r')
                continue;

            if (character == L'\t')
            {
                posX += 4.0 * getAdvance(' ') * characterSpacing;
                continue;
            }

            if (character == L'\n')
                continue;

            if (index != text.count() - 1)
            {
                const auto nextCharacter = text[index + 1];
                const double advance = getAdvance(character, nextCharacter);
                posX += fsScale * advance * characterSpacing;
            }
        }

        return posX;
    }


    bool FFont::loadAndGenerate(const TBufferView<uint8_t>& fontData, EFontAtlasType atlasType,
        const TArray<FCharacterSet>& charSets, IGpuDevice* device)
    {
        msdfgen::FreetypeHandle* freetype = msdfgen::initializeFreetype();
        if (!freetype) return false;

        if (m_PrivateData->handle) msdfgen::destroyFont(m_PrivateData->handle);
        m_PrivateData->handle = msdfgen::loadFontData(freetype, fontData.data(), fontData.count());
        if (!m_PrivateData->handle) return false;

        m_PrivateData->glyphs.clear();
        m_PrivateData->fontGeometry = msdf_atlas::FontGeometry(&m_PrivateData->glyphs);

        msdf_atlas::Charset characterSet;
        for (const auto& [begin, end] : charSets)
            for (uint32_t codePoint = begin; codePoint <= end; codePoint++)
                characterSet.add(codePoint);

        m_PrivateData->fontGeometry.loadCharset(m_PrivateData->handle, 1.0, characterSet);

        constexpr double maxCornerAngle = 3.0;
        for (msdf_atlas::GlyphGeometry& glyph : m_PrivateData->glyphs)
            glyph.edgeColoring(msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);

        msdf_atlas::TightAtlasPacker packer;
        packer.setDimensionsConstraint(msdf_atlas::DimensionsConstraint::POWER_OF_TWO_SQUARE);
        packer.setDimensions(ATLAS_DEFAULT_SIZE, ATLAS_DEFAULT_SIZE);
        packer.setPixelRange(4);
        packer.setMiterLimit(100);
        if (packer.pack(m_PrivateData->glyphs.data(), m_PrivateData->glyphs.size()) != 0)
            return false;

        switch (atlasType)
        {
        case EFontAtlasType::None:
            return false;
        case EFontAtlasType::Bitmap:
            {
                msdf_atlas::ImmediateAtlasGenerator<float, 1, msdf_atlas::scanlineGenerator, msdf_atlas::BitmapAtlasStorage<uint8_t, 1>> generator(ATLAS_DEFAULT_SIZE, ATLAS_DEFAULT_SIZE);
                msdf_atlas::GeneratorAttributes attributes;
                attributes.config.errorCorrection.mode = msdfgen::ErrorCorrectionConfig::INDISCRIMINATE;
                attributes.config.errorCorrection.distanceCheckMode = msdfgen::ErrorCorrectionConfig::ALWAYS_CHECK_DISTANCE;
                attributes.scanlinePass = true;
                generator.setAttributes(attributes);
                generator.setThreadCount(8);
                generator.generate(m_PrivateData->glyphs.data(), m_PrivateData->glyphs.size());

                const msdfgen::BitmapConstRef<uint8_t, 1>& bitmap = generator.atlasStorage();

                FTextureDesc textureDesc = FTextureDesc::texture2D(ATLAS_DEFAULT_SIZE, ATLAS_DEFAULT_SIZE, EFormat::R8_UNORM);
                m_AtlasTexture = device->createTexture(textureDesc);
                if (!TextureUtils::uploadTextureData(device, m_AtlasTexture, 0, 0, bitmap.pixels, ATLAS_DEFAULT_SIZE * ATLAS_DEFAULT_SIZE))
                    return false;
            }
            break;
        case EFontAtlasType::SDF:
            {
                msdf_atlas::ImmediateAtlasGenerator<float, 1, msdf_atlas::sdfGenerator, msdf_atlas::BitmapAtlasStorage<uint8_t, 1>> generator(ATLAS_DEFAULT_SIZE, ATLAS_DEFAULT_SIZE);
                msdf_atlas::GeneratorAttributes attributes;
                attributes.config.errorCorrection.mode = msdfgen::ErrorCorrectionConfig::INDISCRIMINATE;
                attributes.config.errorCorrection.distanceCheckMode = msdfgen::ErrorCorrectionConfig::ALWAYS_CHECK_DISTANCE;
                attributes.scanlinePass = true;
                generator.setAttributes(attributes);
                generator.setThreadCount(8);
                generator.generate(m_PrivateData->glyphs.data(), m_PrivateData->glyphs.size());

                const msdfgen::BitmapConstRef<uint8_t, 1>& bitmap = generator.atlasStorage();

                FTextureDesc textureDesc = FTextureDesc::texture2D(ATLAS_DEFAULT_SIZE, ATLAS_DEFAULT_SIZE, EFormat::R8_UNORM);
                m_AtlasTexture = device->createTexture(textureDesc);
                if (!TextureUtils::uploadTextureData(device, m_AtlasTexture, 0, 0, bitmap.pixels, ATLAS_DEFAULT_SIZE * ATLAS_DEFAULT_SIZE))
                    return false;
            }
            break;
        case EFontAtlasType::MSDF:
            {
                msdf_atlas::ImmediateAtlasGenerator<float, 4, msdf_atlas::mtsdfGenerator, msdf_atlas::BitmapAtlasStorage<uint8_t, 4>> generator(ATLAS_DEFAULT_SIZE, ATLAS_DEFAULT_SIZE);
                msdf_atlas::GeneratorAttributes attributes;
                attributes.config.errorCorrection.mode = msdfgen::ErrorCorrectionConfig::INDISCRIMINATE;
                attributes.config.errorCorrection.distanceCheckMode = msdfgen::ErrorCorrectionConfig::ALWAYS_CHECK_DISTANCE;
                attributes.scanlinePass = true;
                generator.setAttributes(attributes);
                generator.setThreadCount(8);
                generator.generate(m_PrivateData->glyphs.data(), m_PrivateData->glyphs.size());

                const msdfgen::BitmapConstRef<uint8_t, 4>& bitmap = generator.atlasStorage();

                FTextureDesc textureDesc = FTextureDesc::texture2D(ATLAS_DEFAULT_SIZE, ATLAS_DEFAULT_SIZE, EFormat::R8G8B8A8_UNORM);
                m_AtlasTexture = device->createTexture(textureDesc);
                if (!TextureUtils::uploadTextureData(device, m_AtlasTexture, 0, 0, bitmap.pixels, ATLAS_DEFAULT_SIZE * ATLAS_DEFAULT_SIZE * 4))
                    return false;
                break;
            }
        default: return false;
        }

        m_AtlasType = atlasType;
        return true;
    }

    bool FFont::loadAndGenerate(const FStringView filepath, EFontAtlasType atlasType, const TArray<FCharacterSet>& charSets, IGpuDevice* device)
    {
        const TArray<uint8_t> fileContent = FileUtils::readToBuffer(filepath);
        if (filepath.isEmpty()) return false;
        return loadAndGenerate(TBufferView<uint8_t>(fileContent.data(), filepath.count()), atlasType, charSets, device);
    }

    bool FFont::loadFromAtlas(FStringView atlasFilepath, FStringView fontDataFilepath)
    {
        return false;
    }

    void FFont::destroy()
    {
        m_AtlasTexture = nullptr;
        if (m_PrivateData->handle) msdfgen::destroyFont(m_PrivateData->handle);
        m_PrivateData->handle = nullptr;
    }
}
