#include "Luma/IO/MemoryStream.h"

namespace Luma
{
    FMemoryStream::FMemoryStream(const TBufferView<uint8_t>& buffer): FStream(EOpenModeBits::None), m_Buffer(buffer)
    {
        m_Opened = true;
    }

    FStream::SizeType FMemoryStream::readRaw(void* outBuffer, const SizeType size)
    {
        if(!m_Opened) return EndOfFile;
        memcpy(outBuffer, &m_Buffer[m_Position], size);
        m_Position += (OffsetType)size;
        return size;
    }

    FStream::SizeType FMemoryStream::writeRaw(const void* inBuffer, const SizeType size)
    {
        if(!m_Opened) return EndOfFile;
        if(m_Position + size > m_Buffer.count()) return EndOfFile;

        memcpy((void*)&m_Buffer[m_Position], inBuffer, size);
        m_Position += (OffsetType)size;
        return size;
    }

    bool FMemoryStream::seek(const ESeek seek, const OffsetType offset)
    {
        if(!m_Opened) return false;
        switch (seek) {
        case ESeek::Begin:
            if(offset < 0) return false;
            m_Position = offset;
            return true;
        case ESeek::Current:
            if(m_Position + offset < 0) return false;
            m_Position += offset;
            return true;
        case ESeek::End:
            if(offset > 0) return false;
            m_Position = (OffsetType)m_Buffer.count() + offset;
            return true;
        }
        return false;
    }

    FStream::OffsetType FMemoryStream::tell() const
    {
        return m_Opened ? m_Position : OffsetType(~0);
    }

    void FMemoryStream::close()
    {
        FStream::close();
    }

    bool FMemoryStream::isGood() const
    {
        return m_Position < (OffsetType)m_Buffer.count();
    }
}
