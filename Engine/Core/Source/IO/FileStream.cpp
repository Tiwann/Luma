#include "Luma/IO/FileStream.h"
#include "Luma/Containers/StringConversion.h"
#include <cstdio>

namespace Luma
{
    static StringView getMode(const OpenModeFlags openMode)
    {
        switch (openMode.as<uint32_t>()) {
        case (uint32_t)OpenMode::Read: return "r";
        case (uint32_t)OpenMode::Write: return "w";
        case (uint32_t)OpenMode::ReadText: return "r";
        case (uint32_t)OpenMode::ReadBinary: return "rb";
        case (uint32_t)OpenMode::WriteText: return "w";
        case (uint32_t)OpenMode::WriteBinary: return "wb";
        case (uint32_t)OpenMode::ReadWriteText: return "w+";
        case (uint32_t)OpenMode::ReadWriteBinary: return "wb+";
        default: return "";
        }
    }
    
    FileStream::FileStream(StringView filepath, const OpenModeFlags openMode)
        : Stream(openMode), m_Filepath(std::move(filepath))
    {
        open(filepath, openMode);
    }

    bool FileStream::open(const StringView& filepath, const OpenModeFlags openMode)
    {
#ifdef LUMA_PLATFORM_WINDOWS
        StringBase<wchar_t> wideFilepath = stringConvert<wchar_t, char>(filepath);
        StringBase<wchar_t> wideMode = stringConvert<wchar_t, char>(getMode(openMode));
        m_Handle = _wfopen(*wideFilepath, *wideMode);
        m_Opened = m_Handle;
#else
        m_Handle = fopen(*m_Filepath, *getMode(openMode));
        m_Opened = m_Handle;
#endif
        return m_Opened = m_Handle;
    }

    Stream::SizeType FileStream::readRaw(void* outBuffer, const SizeType size)
    {
        return fread(outBuffer, 1, size, m_Handle);
    }

    Stream::SizeType FileStream::writeRaw(const void* inBuffer, const SizeType size)
    {
        return fwrite(inBuffer, 1, size, m_Handle);
    }

    bool FileStream::seek(const Seek seekMode, const OffsetType offset)
    {
        constexpr auto getSeekMode = [](const Seek s) constexpr -> int
        {
            return s == Seek::Begin ? SEEK_SET : s == Seek::Current ? SEEK_CUR : SEEK_END;
        };
            
        const int result = fseek(m_Handle, offset, getSeekMode(seekMode));
        return result == 0;
    }

    Stream::OffsetType FileStream::tell() const
    {
        return ftell(m_Handle);
    }

    void FileStream::close()
    {
        Stream::close();
        (void)fclose(m_Handle);
        m_Handle = nullptr;
    }

    Stream::OffsetType FileStream::getSize()
    {
        seek(Seek::End, 0);
        const OffsetType result = tell();
        seek(Seek::Begin, 0);
        return result;
    }

    bool FileStream::isGood() const
    {
        return !feof(m_Handle);
    }


    FStandardStream FStandardStream::StandardIn = FStandardStream(stdin, OpenMode::Read | OpenMode::Text);
    FStandardStream FStandardStream::StandardOut = FStandardStream(stdout, OpenMode::Write | OpenMode::Text);
    FStandardStream FStandardStream::StandardError = FStandardStream(stderr, OpenMode::Write | OpenMode::Text);
    
    FStandardStream::FStandardStream(FILE* handle, const OpenModeFlags& openMode)
        : Stream(openMode), m_Handle(handle)
    {
        m_Opened = m_Handle;
    }

    Stream::SizeType FStandardStream::readRaw(void* outBuffer, SizeType size)
    {
        return -1;
    }

    Stream::SizeType FStandardStream::writeRaw(const void* inBuffer, const SizeType size)
    {
        return fprintf(m_Handle, "%*s", int(size), (const char*)inBuffer);
    }

    bool FStandardStream::seek(Seek seekMode, OffsetType offset)
    {
        return false;
    }

    Stream::OffsetType FStandardStream::tell() const
    {
        return -1;
    }

    void FStandardStream::close()
    {
        Stream::close();
        (void)fclose(m_Handle);
        m_Handle = nullptr;
    }

    bool FStandardStream::isGood() const
    {
        return !feof(m_Handle);
    }
}
