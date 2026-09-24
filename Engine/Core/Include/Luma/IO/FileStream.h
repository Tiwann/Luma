#pragma once
#include "Stream.h"
#include "Luma/Containers/StringView.h"

namespace Luma
{
    class FileStream final : public Stream
    {
    public:
        FileStream() = default;
        explicit FileStream(StringView filepath, OpenModeFlags openMode);
        
        bool open(const StringView& filepath, OpenModeFlags openMode);
        SizeType readRaw(void* outBuffer, SizeType size) override;
        SizeType writeRaw(const void* inBuffer, SizeType size) override;
        bool seek(Seek seekMode, OffsetType offset) override;
        OffsetType tell() const override;
        void close() override;
        OffsetType getSize();
        bool isGood() const override;

    private:
        StringView m_Filepath;
        FILE* m_Handle = nullptr;
    };

    class FStandardStream final : public Stream
    {
    public:
        
        SizeType readRaw(void* outBuffer, SizeType size) override;
        SizeType writeRaw(const void* inBuffer, SizeType size) override;
        bool seek(Seek seekMode, OffsetType offset) override;
        OffsetType tell() const override;
        void close() override;
        bool isGood() const override;
        
        static FStandardStream StandardOut;
        static FStandardStream StandardIn;
        static FStandardStream StandardError;
        
    protected:
        explicit FStandardStream(FILE* handle, const OpenModeFlags& openMode);
    
    private:
        FILE* m_Handle = nullptr;
    };
}
