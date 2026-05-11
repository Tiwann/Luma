#pragma once
#include "Stream.h"

namespace Luma
{
    class FMemoryStream : public FStream
    {
    public:
        explicit FMemoryStream(const TBufferView<uint8_t>& buffer);

        SizeType    readRaw(void* outBuffer, SizeType size) override;
        SizeType    writeRaw(const void* inBuffer, SizeType size) override;
        bool        seek(ESeek seek, OffsetType offset) override;
        OffsetType  tell() const override;
        void        close() override;
        bool        isGood() const override;
    private:
        TBufferView<uint8_t> m_Buffer;
        OffsetType m_Position = 0;
    };
}
