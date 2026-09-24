#pragma once
#include "Stream.h"

namespace Luma
{
    class MemoryStream : public Stream
    {
    public:
        explicit MemoryStream(const BufferView<uint8_t>& buffer);

        SizeType    readRaw(void* outBuffer, SizeType size) override;
        SizeType    writeRaw(const void* inBuffer, SizeType size) override;
        bool        seek(Seek seek, OffsetType offset) override;
        OffsetType  tell() const override;
        void        close() override;
        bool        isGood() const override;
    private:
        BufferView<uint8_t> m_Buffer;
        OffsetType m_Position = 0;
    };
}
