#include "Luma/Containers/StringConversion.h"
#include "Luma/Runtime/Path.h"
#include "Luma/Containers/StringFormat.h"

#include <cstdio>
#include <array>



namespace Luma
{
    static FString captureCommandOutput(const FStringView command)
    {
        std::array<char, 4096> buffer;
        FString result;

        FILE* pipe = popen(*command, "r");
        if (!pipe) return result;

        while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr)
        {
            // append to result
            const char* c = buffer.data();
            while (*c) { result.append(*c); ++c; }
        }
        pclose(pipe);

        // Trim trailing newline
        if (!result.isEmpty() && result[result.count() - 1] == '\n')
            result.resize(result.count() - 1);
        return result;
    }

    FString FPath::openFileDialog(const FStringView title, const FStringView defaultPath, const FDialogFilters& /*filters*/, IWindow& /*owningWindow*/)
    {
        // Try zenity if available
        FString cmd = strfmt("zenity --file-selection --title=\"{}\" --filename=\"{}\" 2>/dev/null", title, defaultPath);
        const FString out = captureCommandOutput(cmd);
        return out;
    }

    FString FPath::saveFileDialog(FStringView title, FStringView defaultPath, const FDialogFilters& /*filters*/, IWindow& /*owningWindow*/)
    {
        FString cmd = strfmt("zenity --file-selection --save --confirm-overwrite --title=\"{}\" --filename=\"{}\" 2>/dev/null", title, defaultPath);
        const FString out = captureCommandOutput(cmd);
        return out;
    }
}
