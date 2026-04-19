#include "Luma/Containers/StringConversion.h"
#include "Luma/Runtime/Path.h"
#include "Luma/Runtime/DesktopWindow.h"

#include <rgfw/rgfw.h>
#include <Windows.h>
#include <filesystem>


namespace Luma
{
    FString FPath::openFileDialog(const FStringView title, const FStringView defaultPath, const FDialogFilters& filters, IWindow& owningWindow)
    {
        HWND* hwnd = static_cast<HWND*>(RGFW_window_getHWND(((FDesktopWindow&)owningWindow).getHandle()));
        if (!hwnd) return {};

        OPENFILENAMEW openFilename = { };
        ZeroMemory(&openFilename, sizeof(OPENFILENAMEW));
        WCHAR szFile[MAX_PATH] = { 0 };
        openFilename.hwndOwner = *hwnd;
        openFilename.lStructSize = sizeof(OPENFILENAMEW);
        openFilename.lpstrFile = szFile;
        openFilename.nMaxFile = MAX_PATH;
        openFilename.nFilterIndex = 1;
        openFilename.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_EXPLORER;

        const FString filterFString = filters.getFilterString();
        const FWideString filterWideFString = stringConvert<wchar_t, char>(filterFString);
        openFilename.lpstrFilter = *filterWideFString;

        const FWideString wideDefaultPath = stringConvert<wchar_t, char>(defaultPath);
        openFilename.lpstrInitialDir = *wideDefaultPath;

        const FWideString wideTitle = stringConvert<wchar_t, char>(title);
        openFilename.lpstrTitle = *wideTitle;

        const bool result = GetOpenFileNameW(&openFilename);

        FString resultPath = stringConvert<char, wchar_t>(FWideStringView(openFilename.lpstrFile));
        return result ? *resultPath : "";
    }

    FString FPath::saveFileDialog(FStringView title, FStringView defaultPath, const FDialogFilters& filters, IWindow& owningWindow)
    {
        HWND* hwnd = static_cast<HWND*>(RGFW_window_getHWND(((FDesktopWindow&)owningWindow).getHandle()));
        if (!hwnd) return {};

        OPENFILENAME openFilename = { };
        ZeroMemory(&openFilename, sizeof(OPENFILENAMEW));
        WCHAR szFile[MAX_PATH] = { 0 };
        openFilename.hwndOwner = *hwnd;
        openFilename.lStructSize = sizeof(OPENFILENAME);
        openFilename.lpstrFile = szFile;
        openFilename.nMaxFile = MAX_PATH;
        openFilename.nFilterIndex = 1;
        openFilename.Flags = OFN_CREATEPROMPT | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT | OFN_NOLONGNAMES | OFN_EXTENSIONDIFFERENT | OFN_EXPLORER;

        const FString filterString = filters.getFilterString();
        const FWideString filterWideString = stringConvert<wchar_t, char>(filterString);
        openFilename.lpstrFilter = *filterWideString;

        const FWideString wideDefaultPath = stringConvert<wchar_t, char>(defaultPath);
        openFilename.lpstrInitialDir = *wideDefaultPath;

        const FWideString wideTitle = stringConvert<wchar_t, char>(title);
        openFilename.lpstrTitle = *wideTitle;

        const bool result = GetOpenFileNameW(&openFilename);

        FString resultPath = stringConvert<char, wchar_t>(FWideStringView(openFilename.lpstrFile));
        return result ? *resultPath : "";
    }
}
