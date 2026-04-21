#pragma once
#include "Luma/Containers/StringView.h"
#include "Luma/Memory/RefCounted.h"

namespace Luma
{
    struct IRenderDevice;

    enum class ESemaphoreType
    {
        Binary,
        Timeline
    };

    struct FSemaphoreDesc
    {
        IRenderDevice* device = nullptr;
        ESemaphoreType semaphoreType = ESemaphoreType::Binary;
        uint64_t initialValue = 0;
    };

    struct ISemaphore : IRefCounted<ISemaphore>
    {
        virtual bool initialize(const FSemaphoreDesc& desc) = 0;
        virtual void destroy() = 0;

        virtual uint64_t getValue() const = 0;
        virtual void signal(uint64_t value) = 0;
        virtual void wait(uint64_t value, uint64_t timeout) = 0;
        virtual void setName(FStringView name) = 0;
    protected:
        ESemaphoreType m_SemaphoreType = ESemaphoreType::Binary;
    };
}
