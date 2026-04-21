#pragma once
#include "Luma/Memory/RefCounted.h"
#include <cstdint>

#include "Luma/Containers/StringView.h"

namespace Luma
{
    struct IRenderDevice;

    struct FFenceDesc
    {
        IRenderDevice* device = nullptr;
        bool signaled = false;
    };

    static constexpr uint64_t FENCE_WAIT_INFINITE = 1'000'000'000;

    struct IFence : IRefCounted<IFence>
    {
        IFence() = default;
        ~IFence() override = default;

        virtual bool initialize(const FFenceDesc& fenceDesc) = 0;
        virtual void destroy() = 0;
        virtual void wait(uint64_t timeoutNs) = 0;
        virtual void reset() = 0;
        virtual void setName(FStringView name) {}
    };
}
