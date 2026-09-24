#include "Luma/Runtime/Path.h"
#include "Luma/Containers/StringConversion.h"
#include <Windows.h>


namespace Luma
{
    StringView Path::getUserDirectory()
    {
        static String userDirectory;
        if (userDirectory.isEmpty())
        {
            WideString::CharacterType buffer[MAX_PATH] = {0};
            const DWORD length = GetEnvironmentVariableW(L"USERPROFILE", buffer, MAX_PATH);
            userDirectory = stringConvert<char, wchar_t>(WideStringView(buffer, length));
        }

        return userDirectory;
    }

    StringView Path::getDocumentsDirectory()
    {
        static String documentsDirectory = combine(getUserDirectory(), "Documents");
        return documentsDirectory;
    }

    StringView Path::getMusicDirectory()
    {
        static String musicDirectory = combine(getUserDirectory(), "Music");
        return musicDirectory;
    }

    StringView Path::getDownloadsDirectory()
    {
        static String downloadsDirectory = combine(getUserDirectory(), "Downloads");
        return downloadsDirectory;
    }

    StringView Path::getDesktopDirectory()
    {
        static String desktopDirectory = combine(getUserDirectory(), "Desktop");
        return desktopDirectory;
    }
}
