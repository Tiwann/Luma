#include "Luma/IO/FileStream.h"
#include "Luma/Containers/StringConversion.h"
#include <cstdio>

namespace Luma
{
    static FStringView getMode(const FOpenModeFlags openMode)
    {
        switch (openMode.as<uint32_t>()) {
        case (uint32_t)EOpenModeBits::Read: return "r";
        case (uint32_t)EOpenModeBits::Write: return "w";
        case (uint32_t)EOpenModeBits::ReadText: return "r";
        case (uint32_t)EOpenModeBits::ReadBinary: return "rb";
        case (uint32_t)EOpenModeBits::WriteText: return "w";
        case (uint32_t)EOpenModeBits::WriteBinary: return "wb";
        case (uint32_t)EOpenModeBits::ReadWriteText: return "w+";
        case (uint32_t)EOpenModeBits::ReadWriteBinary: return "wb+";
        default: return "";
        }
    }
    
    FFileStream::FFileStream(FStringView filepath, const FOpenModeFlags openMode)
        : FStream(openMode), m_Filepath(std::move(filepath))
    {
        open(filepath, openMode);
    }

    bool FFileStream::open(const FStringView& filepath, const FOpenModeFlags openMode)
    {
#ifdef LUMA_PLATFORM_WINDOWS
        TString<wchar_t> wideFilepath = stringConvert<wchar_t, char>(filepath);
        TString<wchar_t> wideMode = stringConvert<wchar_t, char>(getMode(openMode));
        m_Handle = _wfopen(*wideFilepath, *wideMode);
        m_Opened = m_Handle;
#else
        m_Handle = fopen(*m_Filepath, *getMode(openMode));
        m_Opened = m_Handle;
#endif
        return m_Opened = m_Handle;
    }

    FStream::SizeType FFileStream::readRaw(void* outBuffer, const SizeType size)
    {
        return fread(outBuffer, 1, size, m_Handle);
    }

    FStream::SizeType FFileStream::writeRaw(const void* inBuffer, const SizeType size)
    {
        return fwrite(inBuffer, 1, size, m_Handle);
    }

    bool FFileStream::seek(const ESeek seekMode, const OffsetType offset)
    {
        constexpr auto getSeekMode = [](const ESeek s) constexpr -> int
        {
            return s == ESeek::Begin ? SEEK_SET : s == ESeek::Current ? SEEK_CUR : SEEK_END;
        };
            
        const int result = fseek(m_Handle, offset, getSeekMode(seekMode));
        return result == 0;
    }

    FStream::OffsetType FFileStream::tell() const
    {
        return ftell(m_Handle);
    }

    void FFileStream::close()
    {
        FStream::close();
        (void)fclose(m_Handle);
        m_Handle = nullptr;
    }

    FStream::OffsetType FFileStream::getSize()
    {
        seek(ESeek::End, 0);
        const OffsetType result = tell();
        seek(ESeek::Begin, 0);
        return result;
    }

    bool FFileStream::isGood() const
    {
        return !feof(m_Handle);
    }


    FStandardStream FStandardStream::StandardIn = FStandardStream(stdin, EOpenModeBits::Read | EOpenModeBits::Text);
    FStandardStream FStandardStream::StandardOut = FStandardStream(stdout, EOpenModeBits::Write | EOpenModeBits::Text);
    FStandardStream FStandardStream::StandardError = FStandardStream(stderr, EOpenModeBits::Write | EOpenModeBits::Text);
    
    FStandardStream::FStandardStream(FILE* handle, const FOpenModeFlags& openMode)
        : FStream(openMode), m_Handle(handle)
    {
        m_Opened = m_Handle;
    }

    FStream::SizeType FStandardStream::readRaw(void* outBuffer, SizeType size)
    {
        return -1;
    }

    FStream::SizeType FStandardStream::writeRaw(const void* inBuffer, const SizeType size)
    {
        return fprintf(m_Handle, "%*s", int(size), (const char*)inBuffer);
    }

    bool FStandardStream::seek(ESeek seekMode, OffsetType offset)
    {
        return false;
    }

    FStream::OffsetType FStandardStream::tell() const
    {
        return -1;
    }

    void FStandardStream::close()
    {
        FStream::close();
        (void)fclose(m_Handle);
        m_Handle = nullptr;
    }

    bool FStandardStream::isGood() const
    {
        return !feof(m_Handle);
    }
}
