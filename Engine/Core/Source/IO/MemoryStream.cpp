#include "Luma/IO/MemoryStream.h"

namespace Luma
{
    MemoryStream::MemoryStream(const BufferView<uint8_t>& buffer): Stream(OpenMode::None), m_Buffer(buffer)
    {
        m_Opened = true;
    }

    Stream::SizeType MemoryStream::readRaw(void* outBuffer, const SizeType size)
    {
        if(!m_Opened) return EndOfFile;
        memcpy(outBuffer, &m_Buffer[m_Position], size);
        m_Position += (OffsetType)size;
        return size;
    }

    Stream::SizeType MemoryStream::writeRaw(const void* inBuffer, const SizeType size)
    {
        if(!m_Opened) return EndOfFile;
        if(m_Position + size > m_Buffer.count()) return EndOfFile;

        memcpy((void*)&m_Buffer[m_Position], inBuffer, size);
        m_Position += (OffsetType)size;
        return size;
    }

    bool MemoryStream::seek(const Seek seek, const OffsetType offset)
    {
        if(!m_Opened) return false;
        switch (seek) {
        case Seek::Begin:
            if(offset < 0) return false;
            m_Position = offset;
            return true;
        case Seek::Current:
            if(m_Position + offset < 0) return false;
            m_Position += offset;
            return true;
        case Seek::End:
            if(offset > 0) return false;
            m_Position = (OffsetType)m_Buffer.count() + offset;
            return true;
        }
        return false;
    }

    Stream::OffsetType MemoryStream::tell() const
    {
        return m_Opened ? m_Position : OffsetType(~0);
    }

    void MemoryStream::close()
    {
        Stream::close();
    }

    bool MemoryStream::isGood() const
    {
        return m_Position < (OffsetType)m_Buffer.count();
    }
}
