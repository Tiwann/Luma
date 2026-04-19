#pragma once
#include "Luma/Containers/String.h"
#include "Luma/Containers/Array.h"

namespace Luma
{
    class FDialogFilters
    {
    public:
        struct FFilter
        {
            FString name;
            TArray<FString> extensions;
            bool operator==(const FFilter& other) const;
        };
        static const FFilter All;
        static const FFilter WAV;
        static const FFilter OGG;
        static const FFilter MP3;
        static const FFilter FLAC;
        static const FFilter PNG;
        static const FFilter WEBP;
        static const FFilter JPEG;
        static const FFilter TARGA;
        static const FFilter DDS;
        static const FFilter ASTC;
        static const FFilter BMP;
        static const FFilter FBX;
        static const FFilter GLTF;
        static const FFilter GLB;
        static const FFilter OBJ;
        static const FFilter DAE;
        static const FFilter TTF;
        static const FFilter OTF;
    public:
        FDialogFilters();
        FDialogFilters(const FString& name, const TArray<FString>& extensions);
        FDialogFilters(const FFilter& filter);
        FDialogFilters(const TArray<FFilter>& filters);
        ~FDialogFilters();

        void addFilter(const FString& name, const TArray<FString>& extensions);
        void addFilter(const FFilter& filter);

        template<typename... Ext>
        void addFilter(const FString& name, Ext&&... extensions)
        {
            addFilter(name, { std::forward<Ext>(extensions)... });
        }

        void removeFilter(const FString& name, const TArray<FString>& extensions);
        void removeFilter(const FFilter& filter);
        FFilter* getFilter(const FString& name) const;
        
        FString getFilterString() const;
        
        static const FDialogFilters AudioFilters;
        static const FDialogFilters ModelFilters;
        static const FDialogFilters ImageFilters;
        static const FDialogFilters FontFilters;
    private:
        TArray<FFilter> m_Filters;
    };
}
