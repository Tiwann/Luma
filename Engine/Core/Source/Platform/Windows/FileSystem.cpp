#include "Luma/Runtime/Path.h"
#include "Luma/Containers/StringConversion.h"
#include <Windows.h>


namespace Luma
{
    FStringView FPath::getUserDirectory()
    {
        static FString userDirectory;
        if (userDirectory.isEmpty())
        {
            FWideString::CharacterType buffer[MAX_PATH] = {0};
            const DWORD length = GetEnvironmentVariableW(L"USERPROFILE", buffer, MAX_PATH);
            userDirectory = stringConvert<char, wchar_t>(FWideStringView(buffer, length));
        }

        return userDirectory;
    }

    FStringView FPath::getDocumentsDirectory()
    {
        static FString documentsDirectory = combine(getUserDirectory(), "Documents");
        return documentsDirectory;
    }

    FStringView FPath::getMusicDirectory()
    {
        static FString musicDirectory = combine(getUserDirectory(), "Music");
        return musicDirectory;
    }

    FStringView FPath::getDownloadsDirectory()
    {
        static FString downloadsDirectory = combine(getUserDirectory(), "Downloads");
        return downloadsDirectory;
    }

    FStringView FPath::getDesktopDirectory()
    {
        static FString desktopDirectory = combine(getUserDirectory(), "Desktop");
        return desktopDirectory;
    }
}
