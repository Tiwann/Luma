#pragma once
#include "Luma/Core/Export.h"
#include "Luma/Containers/String.h"
#include "Luma/Containers/StringView.h"
#include "Luma/Runtime/DialogFilters.h"

namespace Luma
{
    struct Window;

    struct LUMA_CORE_API Path
    {
#ifdef LUMA_PLATFORM_WINDOWS
        template<Character T>
        static constexpr StringBase<T>::CharacterType Separator = '\\';

        template<Character T>
        static constexpr StringBase<T>::CharacterType OtherSeparator = '/';
#else
        template<Character T>
        static constexpr StringBase<T>::CharacterType Separator = '/';

        template<Character T>
        static constexpr StringBase<T>::CharacterType OtherSeparator = '\\';
#endif

        static String combine(StringView path, StringView other);

        template<typename... Args>
        static String combine(const StringView path, const StringView other, const Args&... args)
        {
            return combine(combine(path, other), args...);
        }


        static StringView getEngineDir();
        static String getEngineAssetsDir();
        static String getEngineAssetPath(StringView filepath);
        static String getEngineShadersDir();
        static String getEngineShaderPath(StringView filepath);
#ifdef LUMA_CLIENT
        static StringView getExeDir()
        {
            return LUMA_APPLICATION_DIR;
        }

        static String getAssetPath(const StringView filepath)
        {
            return combine(LUMA_APPLICATION_DIR, "Assets", filepath);
        }
#endif

        static StringView getUserDirectory();
        static StringView getDocumentsDirectory();
        static StringView getMusicDirectory();
        static StringView getDownloadsDirectory();
        static StringView getDesktopDirectory();

        static String openFileDialog(StringView title, StringView defaultPath, const FDialogFilters& filters, Window& owningWindow);
        static String saveFileDialog(StringView title, StringView defaultPath, const FDialogFilters& filters, Window& owningWindow);
        static bool exists(StringView path);
        static bool isFile(StringView path);
        static bool isDirectory(StringView path);
        static Array<String> getFiles(StringView path);
        static StringView getFilename(StringView filepath);
        static StringView getExtension(StringView filepath);
        static StringView getDirectory(StringView filepath);
        static StringView getFilenameWithoutExtension(StringView filepath);
    };
}
