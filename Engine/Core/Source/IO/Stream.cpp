#include "Luma/IO/Stream.h"
#include "Luma/Math/Functions.h"

namespace Luma
{
    bool IStream::isOpened() const
    {
        return m_Opened;
    }

    IStream::SizeType IStream::readLine(FString& outLine)
    {
        if (!isGood()) return isGood() ? 0 : EndOfFile;

        SizeType count = 0;
        char buffer[1024];

        while (isGood())
        {
            char character;
            read(character);
            if (character == '\n')
                break;

            if (character == '\r')
            {
                const OffsetType position = tell();
                read(character);
                if (isGood() && character == '\n')
                    break;

                seek(ESeek::Begin, position);
                break;
            }

            buffer[count] = character;
            count++;
        }

        if (count == 0) return isGood() ? 0 : EndOfFile;

        outLine = std::move(buffer);
        return isGood() ? outLine.count() : EndOfFile;
    }

    void IStream::close()
    {
        m_Opened = false;
    }

    IStream::SizeType IStream::read(char& outChar)
    {
        return readRaw(&outChar, sizeof(char));
    }

    IStream::SizeType IStream::read(int8_t& outInt)
    {
        return readRaw(&outInt, sizeof(int8_t));
    }

    IStream::SizeType IStream::read(int16_t& outInt)
    {
        return readRaw(&outInt, sizeof(int16_t));
    }

    IStream::SizeType IStream::read(int32_t& outInt)
    {
        return readRaw(&outInt, sizeof(int32_t));
    }

    IStream::SizeType IStream::read(int64_t& outInt)
    {
        return readRaw(&outInt, sizeof(int64_t));
    }

    IStream::SizeType IStream::read(uint8_t& outInt)
    {
        return readRaw(&outInt, sizeof(uint8_t));
    }

    IStream::SizeType IStream::read(uint16_t& outInt)
    {
        return readRaw(&outInt, sizeof(uint16_t));
    }

    IStream::SizeType IStream::read(uint32_t& outInt)
    {
        return readRaw(&outInt, sizeof(uint32_t));
    }

    IStream::SizeType IStream::read(uint64_t& outInt)
    {
        return readRaw(&outInt, sizeof(uint64_t));
    }

    IStream::SizeType IStream::read(float& outFloat)
    {
        return readRaw(&outFloat, sizeof(float));
    }

    IStream::SizeType IStream::read(double& outDouble)
    {
        return readRaw(&outDouble, sizeof(double));
    }

    IStream::SizeType IStream::read(FString& str, const SizeType count)
    {
        const SizeType bytesToRead = max(count, str.count());
        const SizeType bytesRead = readRaw(*str, bytesToRead);
        return bytesRead;
    }

    IStream::SizeType IStream::write(const char value)
    {
        return writeRaw(&value, sizeof(char));
    }

    IStream::SizeType IStream::write(int8_t value)
    {
        return writeRaw(&value, sizeof(int8_t));
    }

    IStream::SizeType IStream::write(int16_t value)
    {
        return writeRaw(&value, sizeof(int16_t));
    }

    IStream::SizeType IStream::write(int32_t value)
    {
        return writeRaw(&value, sizeof(int32_t));
    }

    IStream::SizeType IStream::write(int64_t value)
    {
        return writeRaw(&value, sizeof(int64_t));
    }

    IStream::SizeType IStream::write(uint8_t value)
    {
        return writeRaw(&value, sizeof(uint8_t));
    }

    IStream::SizeType IStream::write(uint16_t value)
    {
        return writeRaw(&value, sizeof(uint16_t));
    }

    IStream::SizeType IStream::write(uint32_t value)
    {
        return writeRaw(&value, sizeof(uint32_t));
    }

    IStream::SizeType IStream::write(uint64_t value)
    {
        return writeRaw(&value, sizeof(uint64_t));
    }

    IStream::SizeType IStream::write(const FString& string)
    {
        return writeRaw(*string, string.size());
    }

    IStream::SizeType IStream::write(const FStringView string)
    {
        return writeRaw(*string, string.size());
    }

    IStream::SizeType IStream::write(const float value)
    {
        return writeRaw(&value, sizeof(float));
    }

    IStream::SizeType IStream::write(const double value)
    {
        return writeRaw(&value, sizeof(double));
    }
}
