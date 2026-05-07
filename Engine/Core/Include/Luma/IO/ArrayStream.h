#pragma once
#include "Stream.h"

namespace Luma
{
    class FArrayStream final : public FStream
    {
    public:
        FArrayStream();
        ~FArrayStream() override;

        FArrayStream(const FArrayStream&) = delete;
        FArrayStream& operator=(const FArrayStream&) = delete;
        FArrayStream(FArrayStream&&) = delete;
        FArrayStream& operator=(FArrayStream&&) = delete;

        bool isGood() const override;
        SizeType readRaw(void* outBuffer, SizeType size) override;
        SizeType writeRaw(const void* inBuffer, SizeType size) override;
        bool seek(ESeek seekMode, OffsetType offset) override;
        OffsetType tell() const override;

        TBufferView<uint8_t> getView() const { return { m_Data, m_Size }; }
        const uint8_t* data() const { return m_Data; }
        size_t size() const { return m_Size; }
    private:
        uint8_t* m_Data = nullptr;
        SizeType m_Size = 0;
        SizeType m_Allocated = 0;
        OffsetType m_Position = 0;
    };
}
