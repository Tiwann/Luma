#pragma once
#include "Luma/Runtime/Format.h"
#include <cstdint>

namespace Luma
{
    struct IRenderDevice;
    struct ITexture;

    struct FTextureUploaderDesc
    {
        IRenderDevice* device = nullptr;

    };

    struct FMipUploadData
    {
        EFormat format;
        uint32_t mipLevel = 0;
        const uint8_t* data = nullptr;
        uint64_t dataSize = 0;
    };

    struct FTextureUploadData
    {
        uint32_t arrayIndex = 0;
        FMipUploadData* mipsData = nullptr;
        uint32_t mipCount = 0;
    };

    struct FTextureUploader
    {
        virtual ~FTextureUploader() = default;
        virtual bool initialize(const FTextureUploaderDesc& desc) = 0;
        virtual void destroy() = 0;
        virtual bool upload(ITexture* texture, const FTextureUploadData& uploadData) = 0;
    };
}
