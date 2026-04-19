#include "Luma/Runtime/DialogFilters.h"
#include "Luma/Containers/StringFormat.h"

namespace Luma
{
    FDialogFilters::FDialogFilters() = default;

    FDialogFilters::FDialogFilters(const FString& name, const TArray<FString>& extensions)
    {
       addFilter(name, extensions);
    }

    FDialogFilters::FDialogFilters(const FFilter& filter)
    {
        m_Filters.add(filter);
    }

    FDialogFilters::~FDialogFilters()
    {

    }

    void FDialogFilters::removeFilter(const FString& name, const TArray<FString>& extensions)
    {
        m_Filters.remove({name, extensions});
    }

    void FDialogFilters::removeFilter(const FFilter& filter)
    {
        m_Filters.remove(filter);
    }

    FDialogFilters::FFilter* FDialogFilters::getFilter(const FString& name) const
    {
        FFilter* Result = m_Filters.single([&name](const FFilter& filter) { return filter.name == name; });
        return Result;
    }

    void FDialogFilters::addFilter(const FString& name, const TArray<FString>& extensions)
    {
        m_Filters.add({ name, extensions });
    }

    void FDialogFilters::addFilter(const FFilter& filter)
    {
        m_Filters.add(filter);
    }

    const FDialogFilters::FFilter FDialogFilters::All = {"All Files", {"*"}};
    const FDialogFilters::FFilter FDialogFilters::WAV = {"Waveform Audio", {"wav"}};
    const FDialogFilters::FFilter FDialogFilters::OGG = {"Vorbis", {"ogg"}};
    const FDialogFilters::FFilter FDialogFilters::MP3 = {"MPEG Audio Layer III", {"mp3"}};
    const FDialogFilters::FFilter FDialogFilters::FLAC = {"Free Lossless Audio Codec", {"flac"}};
    const FDialogFilters::FFilter FDialogFilters::PNG = {"Portable Network Graphics", {"png"}};
    const FDialogFilters::FFilter FDialogFilters::WEBP = {"Google Webp", {"webp"}};
    const FDialogFilters::FFilter FDialogFilters::JPEG = {"Joint Photographic Experts Group", {"jpg", "jpeg"}};
    const FDialogFilters::FFilter FDialogFilters::TARGA = {"Truevision Targa", {"tga"}};
    const FDialogFilters::FFilter FDialogFilters::DDS = {"DirectDraw Surface", {"dds"}};
    const FDialogFilters::FFilter FDialogFilters::ASTC = {"Adaptive Scalable Texture", {"astc"}};
    const FDialogFilters::FFilter FDialogFilters::BMP = {"Windows Bitmap", {"bmp"}};
    const FDialogFilters::FFilter FDialogFilters::FBX = {"Autodesk Filmbox", {"fbx"}};
    const FDialogFilters::FFilter FDialogFilters::GLTF = {"GL Transmission Format", {"gltf", "glb"}};
    const FDialogFilters::FFilter FDialogFilters::OBJ = {"Wavefront OBJ", {"obj"}};
    const FDialogFilters::FFilter FDialogFilters::DAE = {"Collada", {"dae"}};
    const FDialogFilters::FFilter FDialogFilters::TTF = {"TrueType Font", {"ttf", "dfont"}};
    const FDialogFilters::FFilter FDialogFilters::OTF = {"OpenType Font", {"otf", "otc", "ttf", "ttc"}};

    bool FDialogFilters::FFilter::operator==(const FFilter& other) const
    {
        return name == other.name && extensions == other.extensions;
    }

    FDialogFilters::FDialogFilters(const TArray<FFilter>& Filters)
    {
        m_Filters = Filters;
    }

    const FDialogFilters FDialogFilters::ImageFilters = TArray {
        {"All Image Formats", {"png", "jpg", "jpeg", "bmp", "tga", "dds", "astc"}},
        PNG, JPEG, BMP, TARGA, DDS, ASTC, All
    };

    const FDialogFilters FDialogFilters::AudioFilters = TArray {
        FFilter{ .name = "All Audio Formats", .extensions = { "wav", "mp3", "ogg", "flac" }},
        WAV, MP3, OGG, FLAC, All
    };

    const FDialogFilters FDialogFilters::ModelFilters = TArray {
        FFilter{ .name = "All Model Formats", .extensions = { "fbx", "gltf", "glb", "obj", "dae" }},
        FBX, GLTF, OBJ, DAE, All
    };

    const FDialogFilters FDialogFilters::FontFilters = TArray {
        FFilter{ .name = "All Font Formats", .extensions = { "ttf", "ttc", "otf", "otc", "dfont" }},
        TTF, OTF
    };

    FString FDialogFilters::getFilterString() const
    {
        TArray<FString> displayNames;
        TArray<FString> extensions;

        for (const FFilter& filter : m_Filters)
        {
            FString extensionsComma;
            FString extensionsSemiColon;
            for (size_t i = 0; i < filter.extensions.count(); i++)
            {
                if (i != filter.extensions.count() - 1)
                {
                    extensionsComma.append(strfmt(".{}, ", filter.extensions[i]));
                    extensionsSemiColon.append(strfmt("*.{};", filter.extensions[i]));
                } else
                {
                    extensionsComma.append(strfmt(".{}", filter.extensions[i]));
                    extensionsSemiColon.append(strfmt("*.{}", filter.extensions[i]));
                }
            }
            displayNames.add(strfmt("{} ({})", filter.name, extensionsComma));
            extensions.add(extensionsSemiColon);
        }

        size_t totalCount = 1;
        for (size_t i = 0; i < m_Filters.count(); i++)
        {
            totalCount += displayNames[i].count() + 1;
            totalCount += extensions[i].count() + 1;
        }

        FString result(totalCount);
        
        size_t currentCount = 0;
        for (size_t i = 0; i < m_Filters.count(); i++)
        {
            const FString& currentDisplayName = displayNames[i];
            const FString& currentExtension = extensions[i];
            memcpy(*result + currentCount, *currentDisplayName, currentDisplayName.count());
            memcpy(*result + currentCount + currentDisplayName.count() + 1 , *currentExtension, currentExtension.count());
            currentCount += currentDisplayName.count() + currentExtension.count() + 1 + 1;
        }
        return result;
    }
}
