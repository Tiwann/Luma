#include "Luma/Runtime/FileUtils.h"
#include "Luma/IO/FileStream.h"

namespace Luma::FileUtils
{
    Array<uint8_t> readToBuffer(const StringView filepath)
    {
        FileStream stream(filepath, OpenMode::ReadBinary);
        if (!stream.isOpened()) return {};

        const size_t fileSize = stream.getSize();
        Array<uint8_t> buffer(fileSize);
        const size_t read = stream.readRaw(buffer.data(), fileSize);
        if (read != fileSize) return {};

        return buffer;
    }

    String readToString(const StringView filepath)
    {
        FileStream stream(filepath, OpenMode::ReadBinary);
        if (!stream.isOpened()) return {};

        const size_t fileSize = stream.getSize();
        String result(fileSize);
        const size_t read = stream.read(result, fileSize);

        if (read != fileSize) return {};
        return result;
    }

    Array<uint8_t> readTillEnd(Stream& stream)
    {
        const size_t position = stream.tell();

        stream.seek(Seek::End, 0);
        const size_t endPosition = stream.tell();
        stream.seek(Seek::Begin, position);

        const size_t bytesToRead = endPosition - position;
        Array<uint8_t> result(bytesToRead);
        const size_t read = stream.readRaw(result.data(), result.size());
        if (read != bytesToRead) return {};
        return result;
    }
}
