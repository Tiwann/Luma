#include "Luma/Runtime/Path.h"

#include <filesystem>

#include "Luma/Containers/StringFormat.h"

namespace Luma
{
    String Path::combine(const StringView path, const StringView other)
    {
        return strfmt("{}{}{}", path, Separator<char>, other).replaceAll(OtherSeparator<char>, Separator<char>);
    }

    StringView Path::getEngineDir()
    {
        return LUMA_ENGINE_ROOT_DIR;
    }

    String Path::getEngineAssetsDir()
    {
        return combine(getEngineDir(), "Assets");
    }

    String Path::getEngineAssetPath(const StringView filepath)
    {
        return combine(getEngineAssetsDir(), filepath);
    }

    String Path::getEngineShadersDir()
    {
        return combine(getEngineAssetsDir(), "Shaders/Source");
    }

    String Path::getEngineShaderPath(StringView filepath)
    {
        return combine(getEngineShadersDir(), filepath);
    }

    StringView Path::getFilename(const StringView filepath)
    {
        const auto sep = filepath.findLast(Separator<char>);
        if (sep == -1)
            return filepath;

        return filepath.subview(sep + 1);
    }

    StringView Path::getExtension(const StringView filepath)
    {
        const auto dot = filepath.findLast('.');
        const auto sep = filepath.findLast(Separator<char>);

        if (dot == -1 || (sep != -1 && dot < sep))
            return StringView::empty();

        return filepath.subview(dot + 1);
    }

    StringView Path::getDirectory(const StringView filepath)
    {
        const auto sep = filepath.findLast(Separator<char>);
        if (sep == -1)
            return StringView::empty();

        return filepath.subview(0, sep);
    }

    StringView Path::getFilenameWithoutExtension(const StringView filepath)
    {
        const auto sep = filepath.findLast(Separator<char>);
        const auto dot = filepath.findLast('.');

        const size_t start = (sep == -1) ? 0 : sep + 1;

        if (dot == -1 || dot < start)
            return filepath.subview(start);

        return filepath.subview(start, dot - start);
    }

    bool Path::exists(StringView path)
    {
        return std::filesystem::exists({*path});
    }

    bool Path::isFile(StringView path)
    {
        return !isDirectory(path);
    }

    bool Path::isDirectory(StringView path)
    {
        return std::filesystem::is_directory({*path});
    }

    Array<String> Path::getFiles(StringView path)
    {
        if (!isDirectory(path)) return {};
        std::filesystem::path directory(*path);
        Array<String> files;
        for (auto it : std::filesystem::directory_iterator(directory))
            files.add(it.path().string().c_str());
        return files;
    }
}
