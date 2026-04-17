#pragma once
#include <cstdint>

namespace Luma
{
    struct IRenderDevice;

    struct FFenceDesc
    {
        IRenderDevice* device = nullptr;
    };

    static constexpr uint64_t FENCE_WAIT_INFINITE = 1'000'000'000;

    struct IFence
    {
        IFence() = default;
        virtual ~IFence() = default;

        virtual bool initialize(const FFenceDesc& fenceDesc) = 0;
        virtual void destroy() = 0;
        virtual void wait(uint64_t timeoutNs) = 0;
        virtual void reset() = 0;
    };
}
