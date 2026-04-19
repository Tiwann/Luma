#include "Luma/Runtime/Path.h"

#include <filesystem>

#include "Luma/Containers/StringFormat.h"

namespace Luma
{
    FString FPath::combine(const FStringView path, const FStringView other)
    {
        return strfmt("{}{}{}", path, Separator<char>, other).replaceAll(OtherSeparator<char>, Separator<char>);
    }

    FStringView FPath::getEngineDirectory()
    {
        return LUMA_ENGINE_ROOT_DIR;
    }

    FString FPath::getEngineAssetsDirectory()
    {
        return combine(getEngineDirectory(), "Assets");
    }

    FString FPath::getEngineAssetPath(const FStringView filepath)
    {
        return combine(getEngineAssetsDirectory(), filepath);
    }

    FStringView FPath::getFilename(const FStringView filepath)
    {
        const auto sep = filepath.findLast(Separator<char>);
        if (sep == -1)
            return filepath;

        return filepath.subview(sep + 1);
    }

    FStringView FPath::getExtension(const FStringView filepath)
    {
        const auto dot = filepath.findLast('.');
        const auto sep = filepath.findLast(Separator<char>);

        if (dot == -1 || (sep != -1 && dot < sep))
            return FStringView::empty();

        return filepath.subview(dot + 1);
    }

    FStringView FPath::getDirectory(const FStringView filepath)
    {
        const auto sep = filepath.findLast(Separator<char>);
        if (sep == -1)
            return FStringView::empty();

        return filepath.subview(0, sep);
    }

    FStringView FPath::getFilenameWithoutExtension(const FStringView filepath)
    {
        const auto sep = filepath.findLast(Separator<char>);
        const auto dot = filepath.findLast('.');

        const size_t start = (sep == -1) ? 0 : sep + 1;

        if (dot == -1 || dot < start)
            return filepath.subview(start);

        return filepath.subview(start, dot - start);
    }

    bool FPath::exists(FStringView path)
    {
        return std::filesystem::exists({*path});
    }

    bool FPath::isFile(FStringView path)
    {
        return !isDirectory(path);
    }

    bool FPath::isDirectory(FStringView path)
    {
        return std::filesystem::is_directory({*path});
    }

    TArray<FString> FPath::getFiles(FStringView path)
    {
        if (!isDirectory(path)) return {};
        std::filesystem::path directory(*path);
        TArray<FString> files;
        for (auto it : std::filesystem::directory_iterator(directory))
            files.add(it.path().string().c_str());
        return files;
    }
}
