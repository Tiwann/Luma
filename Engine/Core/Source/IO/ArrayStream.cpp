#include "Luma/IO/ArrayStream.h"
#include "Luma/Memory/Memory.h"

namespace Luma
{
    static FArrayStream::SizeType getNextSize(const FArrayStream::SizeType size)
    {
        if (size == 0) return 1;
        return size * 2;
    }

    FArrayStream::FArrayStream()
        : FStream(EOpenModeBits::None)
    {
        m_Opened = true;
        m_Data = nullptr;
        m_Allocated = 0;
        m_Size = 0;
    }

    FArrayStream::~FArrayStream()
    {
        Memory::free(m_Data);
        m_Allocated = 0;
        m_Size = 0;
    }

    bool FArrayStream::isGood() const
    {
        return true;
    }

    FStream::SizeType FArrayStream::readRaw(void* outBuffer, SizeType size)
    {
        if(!m_Opened) return -1ULL;
        if (m_Position + size > m_Size) return -1ULL;
        Memory::memcpy(outBuffer, &m_Data[m_Position], size);
        m_Position += (OffsetType)size;
        return size;
    }

    FStream::SizeType FArrayStream::writeRaw(const void* inBuffer, SizeType size)
    {
        if(!m_Opened) return -1ull;

        bool shouldReallocate = false;
        while (m_Position + size > m_Allocated)
        {
            shouldReallocate = true;
            m_Allocated = getNextSize(size);
        }

        if (shouldReallocate)
        {
            uint8_t* newPlace = static_cast<uint8_t*>(Memory::allocate(m_Allocated));
            Memory::memcpy(newPlace, m_Data, m_Size);
            Memory::free(m_Data);
            m_Data = newPlace;
        }

        Memory::memcpy(&m_Data[m_Position], inBuffer, size);
        if (m_Position + size > m_Size)
            m_Size += size - (m_Size - m_Position);
        
        m_Position += (OffsetType)size;
        return size;
    }

    bool FArrayStream::seek(ESeek seekMode, OffsetType offset)
    {
        if(!m_Opened) return false;
        switch (seekMode) {
        case ESeek::Begin:
            if(offset < 0) return false;
            if (offset > (OffsetType)m_Size) return false;
            m_Position = offset;
            return true;
        case ESeek::Current:
            if(m_Position + offset < 0) return false;
            if(m_Position + offset > (OffsetType)m_Size) return false;
            m_Position += offset;
            return true;
        case ESeek::End:
            if(offset > 0) return false;
            if((OffsetType)m_Size + offset < 0) return false;
            m_Position = (OffsetType)m_Size + offset;
            return true;
        }
        return false;
    }

    FStream::OffsetType FArrayStream::tell() const
    {
        return m_Opened ? m_Position : (OffsetType)EndOfFile;
    }
}
