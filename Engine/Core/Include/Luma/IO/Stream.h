#pragma once
#pragma warning(disable:4146)
#include "OpenMode.h"
#include "Seek.h"
#include "Luma/Containers/BufferView.h"
#include "Luma/Containers/StringView.h"

namespace Luma
{
    class FStream
    {
    public:
        using SizeType = uint64_t;
        using OffsetType = off_t;

        static constexpr SizeType EndOfFile = SizeType(~0);

        FStream() = default;
        explicit FStream(const FOpenModeFlags openMode) : m_OpenMode(openMode) {}
        virtual ~FStream() { close(); }

        bool isOpened() const;
        virtual bool isGood() const = 0;

        virtual SizeType readRaw(void* outBuffer, SizeType size) = 0;
        virtual SizeType writeRaw(const void* inBuffer, SizeType size) = 0;
        virtual bool seek(ESeek seekMode, OffsetType offset) = 0;
        virtual OffsetType tell() const = 0;
        virtual void close();

        bool rewind() { seek(ESeek::Begin, 0); return isOpened(); }

        SizeType readLine(FString& outLine);
        SizeType read(FString& str, SizeType count);
        SizeType read(char& outChar);
        SizeType read(int8_t& outInt);
        SizeType read(int16_t& outInt);
        SizeType read(int32_t& outInt);
        SizeType read(int64_t& outInt);
        SizeType read(uint8_t& outInt);
        SizeType read(uint16_t& outInt);
        SizeType read(uint32_t& outInt);
        SizeType read(uint64_t& outInt);
        SizeType read(float& outFloat);
        SizeType read(double& outDouble);

        template<typename Type>
        SizeType readBuffer(TBufferView<Type>& outBuffer)
        {
            return readRaw(outBuffer.data(), outBuffer.count());
        }

        template<typename T>
        SizeType readObject(T& obj)
        {
            return readRaw((void*)&obj, sizeof(T));
        }

        template<Character T>
        SizeType readString(TString<T>& str, SizeType count)
        {
            const SizeType bytesToRead = min(count, str.size());
            const SizeType bytesRead = readRaw(*str, bytesToRead);
            return bytesRead;
        }

        template<typename T>
        SizeType writeObject(const T& obj)
        {
            return writeRaw((void*)&obj, sizeof(T));
        }

        template<Character T>
        SizeType write(const TString<T>& str)
        {
            return writeRaw(*str, str.size());
        }

        SizeType write(char value);
        SizeType write(int8_t value);
        SizeType write(int16_t value);
        SizeType write(int32_t value);
        SizeType write(int64_t value);
        SizeType write(uint8_t value);
        SizeType write(uint16_t value);
        SizeType write(uint32_t value);
        SizeType write(uint64_t value);
        SizeType write(const FString& string);
        SizeType write(FStringView string);
        SizeType write(float value);
        SizeType write(double value);

    protected:
        bool m_Opened = false;
        FOpenModeFlags m_OpenMode = EOpenModeBits::None;
    };
}

