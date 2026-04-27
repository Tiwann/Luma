#pragma once
#include "Luma/Core/Export.h"
#include "Luma/Containers/String.h"
#include "Luma/Containers/StringView.h"
#include "Luma/Runtime/DialogFilters.h"

namespace Luma
{
    struct IWindow;

    struct LUMA_CORE_API FPath
    {
#ifdef LUMA_PLATFORM_WINDOWS
        template<Character T>
        static constexpr TString<T>::CharacterType Separator = '\\';

        template<Character T>
        static constexpr TString<T>::CharacterType OtherSeparator = '/';
#else
        template<Character T>
        static constexpr TString<T>::CharacterType Separator = '/';

        template<Character T>
        static constexpr TString<T>::CharacterType OtherSeparator = '\\';
#endif

        static FString combine(FStringView path, FStringView other);

        template<typename... Args>
        static FString combine(const FStringView path, const FStringView other, const Args&... args)
        {
            return combine(combine(path, other), args...);
        }


        static FStringView getEngineDir();
        static FString getEngineAssetsDir();
        static FString getEngineAssetPath(FStringView filepath);

#ifdef LUMA_CLIENT
        static FStringView getExeDir()
        {
            return LUMA_APPLICATION_DIR;
        }

        static FString getAssetPath(const FStringView filepath)
        {
            return combine(FStringView(LUMA_APPLICATION_DIR), FStringView("Assets"), filepath);
        }
#endif

        static FStringView getUserDirectory();
        static FStringView getDocumentsDirectory();
        static FStringView getMusicDirectory();
        static FStringView getDownloadsDirectory();
        static FStringView getDesktopDirectory();

        static FString openFileDialog(FStringView title, FStringView defaultPath, const FDialogFilters& filters, IWindow& owningWindow);
        static FString saveFileDialog(FStringView title, FStringView defaultPath, const FDialogFilters& filters, IWindow& owningWindow);
        static bool exists(FStringView path);
        static bool isFile(FStringView path);
        static bool isDirectory(FStringView path);
        static TArray<FString> getFiles(FStringView path);
        static FStringView getFilename(FStringView filepath);
        static FStringView getExtension(FStringView filepath);
        static FStringView getDirectory(FStringView filepath);
        static FStringView getFilenameWithoutExtension(FStringView filepath);
    };
}
