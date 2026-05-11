#include "Luma/Runtime/FileUtils.h"
#include "Luma/IO/FileStream.h"

namespace Luma::FileUtils
{
    TArray<uint8_t> readToBuffer(const FStringView filepath)
    {
        FFileStream stream(filepath, EOpenModeBits::ReadBinary);
        if (!stream.isOpened()) return {};

        const size_t fileSize = stream.getSize();
        TArray<uint8_t> buffer(fileSize);
        const size_t read = stream.readRaw(buffer.data(), fileSize);
        if (read != fileSize) return {};

        return buffer;
    }

    FString readToString(const FStringView filepath)
    {
        FFileStream stream(filepath, EOpenModeBits::ReadBinary);
        if (!stream.isOpened()) return {};

        const size_t fileSize = stream.getSize();
        FString result(fileSize);
        const size_t read = stream.read(result, fileSize);

        if (read != fileSize) return {};
        return result;
    }

    TArray<uint8_t> readTillEnd(FStream& stream)
    {
        const size_t position = stream.tell();

        stream.seek(ESeek::End, 0);
        const size_t endPosition = stream.tell();
        stream.seek(ESeek::Begin, position);

        const size_t bytesToRead = endPosition - position;
        TArray<uint8_t> result(bytesToRead);
        const size_t read = stream.readRaw(result.data(), result.size());
        if (read != bytesToRead) return {};
        return result;
    }
}
