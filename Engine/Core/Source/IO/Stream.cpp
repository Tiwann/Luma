#include "Luma/IO/Stream.h"
#include "Luma/Math/Functions.h"

namespace Luma
{
    bool FStream::isOpened() const
    {
        return m_Opened;
    }

    FStream::SizeType FStream::readLine(FString& outLine)
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

    void FStream::close()
    {
        m_Opened = false;
    }

    FStream::SizeType FStream::read(char& outChar)
    {
        return readRaw(&outChar, sizeof(char));
    }

    FStream::SizeType FStream::read(int8_t& outInt)
    {
        return readRaw(&outInt, sizeof(int8_t));
    }

    FStream::SizeType FStream::read(int16_t& outInt)
    {
        return readRaw(&outInt, sizeof(int16_t));
    }

    FStream::SizeType FStream::read(int32_t& outInt)
    {
        return readRaw(&outInt, sizeof(int32_t));
    }

    FStream::SizeType FStream::read(int64_t& outInt)
    {
        return readRaw(&outInt, sizeof(int64_t));
    }

    FStream::SizeType FStream::read(uint8_t& outInt)
    {
        return readRaw(&outInt, sizeof(uint8_t));
    }

    FStream::SizeType FStream::read(uint16_t& outInt)
    {
        return readRaw(&outInt, sizeof(uint16_t));
    }

    FStream::SizeType FStream::read(uint32_t& outInt)
    {
        return readRaw(&outInt, sizeof(uint32_t));
    }

    FStream::SizeType FStream::read(uint64_t& outInt)
    {
        return readRaw(&outInt, sizeof(uint64_t));
    }

    FStream::SizeType FStream::read(float& outFloat)
    {
        return readRaw(&outFloat, sizeof(float));
    }

    FStream::SizeType FStream::read(double& outDouble)
    {
        return readRaw(&outDouble, sizeof(double));
    }

    FStream::SizeType FStream::read(FString& str, const SizeType count)
    {
        const SizeType bytesToRead = max(count, str.count());
        const SizeType bytesRead = readRaw(*str, bytesToRead);
        return bytesRead;
    }

    FStream::SizeType FStream::write(const char value)
    {
        return writeRaw(&value, sizeof(char));
    }

    FStream::SizeType FStream::write(int8_t value)
    {
        return writeRaw(&value, sizeof(int8_t));
    }

    FStream::SizeType FStream::write(int16_t value)
    {
        return writeRaw(&value, sizeof(int16_t));
    }

    FStream::SizeType FStream::write(int32_t value)
    {
        return writeRaw(&value, sizeof(int32_t));
    }

    FStream::SizeType FStream::write(int64_t value)
    {
        return writeRaw(&value, sizeof(int64_t));
    }

    FStream::SizeType FStream::write(uint8_t value)
    {
        return writeRaw(&value, sizeof(uint8_t));
    }

    FStream::SizeType FStream::write(uint16_t value)
    {
        return writeRaw(&value, sizeof(uint16_t));
    }

    FStream::SizeType FStream::write(uint32_t value)
    {
        return writeRaw(&value, sizeof(uint32_t));
    }

    FStream::SizeType FStream::write(uint64_t value)
    {
        return writeRaw(&value, sizeof(uint64_t));
    }

    FStream::SizeType FStream::write(const FString& string)
    {
        return writeRaw(*string, string.size());
    }

    FStream::SizeType FStream::write(const FStringView string)
    {
        return writeRaw(*string, string.size());
    }

    FStream::SizeType FStream::write(const float value)
    {
        return writeRaw(&value, sizeof(float));
    }

    FStream::SizeType FStream::write(const double value)
    {
        return writeRaw(&value, sizeof(double));
    }
}
