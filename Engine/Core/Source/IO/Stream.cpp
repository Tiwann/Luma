#include "Luma/IO/Stream.h"
#include "Luma/Math/Functions.h"

namespace Luma
{
    bool Stream::isOpened() const
    {
        return m_Opened;
    }

    Stream::SizeType Stream::readLine(String& outLine)
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

                seek(Seek::Begin, position);
                break;
            }

            buffer[count] = character;
            count++;
        }

        if (count == 0) return isGood() ? 0 : EndOfFile;

        outLine = std::move(buffer);
        return isGood() ? outLine.count() : EndOfFile;
    }

    void Stream::close()
    {
        m_Opened = false;
    }

    Stream::SizeType Stream::read(char& outChar)
    {
        return readRaw(&outChar, sizeof(char));
    }

    Stream::SizeType Stream::read(int8_t& outInt)
    {
        return readRaw(&outInt, sizeof(int8_t));
    }

    Stream::SizeType Stream::read(int16_t& outInt)
    {
        return readRaw(&outInt, sizeof(int16_t));
    }

    Stream::SizeType Stream::read(int32_t& outInt)
    {
        return readRaw(&outInt, sizeof(int32_t));
    }

    Stream::SizeType Stream::read(int64_t& outInt)
    {
        return readRaw(&outInt, sizeof(int64_t));
    }

    Stream::SizeType Stream::read(uint8_t& outInt)
    {
        return readRaw(&outInt, sizeof(uint8_t));
    }

    Stream::SizeType Stream::read(uint16_t& outInt)
    {
        return readRaw(&outInt, sizeof(uint16_t));
    }

    Stream::SizeType Stream::read(uint32_t& outInt)
    {
        return readRaw(&outInt, sizeof(uint32_t));
    }

    Stream::SizeType Stream::read(uint64_t& outInt)
    {
        return readRaw(&outInt, sizeof(uint64_t));
    }

    Stream::SizeType Stream::read(float& outFloat)
    {
        return readRaw(&outFloat, sizeof(float));
    }

    Stream::SizeType Stream::read(double& outDouble)
    {
        return readRaw(&outDouble, sizeof(double));
    }

    Stream::SizeType Stream::read(String& str, const SizeType count)
    {
        const SizeType bytesToRead = max(count, str.count());
        const SizeType bytesRead = readRaw(*str, bytesToRead);
        return bytesRead;
    }

    Stream::SizeType Stream::write(const char value)
    {
        return writeRaw(&value, sizeof(char));
    }

    Stream::SizeType Stream::write(int8_t value)
    {
        return writeRaw(&value, sizeof(int8_t));
    }

    Stream::SizeType Stream::write(int16_t value)
    {
        return writeRaw(&value, sizeof(int16_t));
    }

    Stream::SizeType Stream::write(int32_t value)
    {
        return writeRaw(&value, sizeof(int32_t));
    }

    Stream::SizeType Stream::write(int64_t value)
    {
        return writeRaw(&value, sizeof(int64_t));
    }

    Stream::SizeType Stream::write(uint8_t value)
    {
        return writeRaw(&value, sizeof(uint8_t));
    }

    Stream::SizeType Stream::write(uint16_t value)
    {
        return writeRaw(&value, sizeof(uint16_t));
    }

    Stream::SizeType Stream::write(uint32_t value)
    {
        return writeRaw(&value, sizeof(uint32_t));
    }

    Stream::SizeType Stream::write(uint64_t value)
    {
        return writeRaw(&value, sizeof(uint64_t));
    }

    Stream::SizeType Stream::write(const String& string)
    {
        return writeRaw(*string, string.size());
    }

    Stream::SizeType Stream::write(const StringView string)
    {
        return writeRaw(*string, string.size());
    }

    Stream::SizeType Stream::write(const float value)
    {
        return writeRaw(&value, sizeof(float));
    }

    Stream::SizeType Stream::write(const double value)
    {
        return writeRaw(&value, sizeof(double));
    }
}
