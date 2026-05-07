#pragma once
#include "Stream.h"
#include "Luma/Containers/StringView.h"

namespace Luma
{
    class FFileStream final : public FStream
    {
    public:
        FFileStream() = default;
        explicit FFileStream(FStringView filepath, FOpenModeFlags openMode);
        
        bool open(const FStringView& filepath, FOpenModeFlags openMode);
        SizeType readRaw(void* outBuffer, SizeType size) override;
        SizeType writeRaw(const void* inBuffer, SizeType size) override;
        bool seek(ESeek seekMode, OffsetType offset) override;
        OffsetType tell() const override;
        void close() override;
        OffsetType getSize();
        bool isGood() const override;

    private:
        FStringView m_Filepath;
        FILE* m_Handle = nullptr;
    };

    class FStandardStream final : public FStream
    {
    public:
        
        SizeType readRaw(void* outBuffer, SizeType size) override;
        SizeType writeRaw(const void* inBuffer, SizeType size) override;
        bool seek(ESeek seekMode, OffsetType offset) override;
        OffsetType tell() const override;
        void close() override;
        bool isGood() const override;
        
        static FStandardStream StandardOut;
        static FStandardStream StandardIn;
        static FStandardStream StandardError;
        
    protected:
        explicit FStandardStream(FILE* handle, const FOpenModeFlags& openMode);
    
    private:
        FILE* m_Handle = nullptr;
    };
}
