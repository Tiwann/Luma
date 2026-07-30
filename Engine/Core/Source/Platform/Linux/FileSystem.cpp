#include "Luma/Runtime/Path.h"
#include "Luma/Containers/StringConversion.h"
#include <cstdlib>

namespace Luma
{
    FStringView FPath::getUserDirectory()
    {
        static FString userDirectory;
        if (userDirectory.isEmpty())
        {
            const char* home = getenv("HOME");
            if (!home) home = "/";
            userDirectory = FString(home);
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
