#pragma once
#include "PipelineStage.h"
#include "Luma/Containers/StringView.h"
#include "Luma/Memory/RefCounted.h"
#include <cstdint>

namespace Luma::RHI
{
    struct Device;

    struct FenceDesc
    {
        Device* device = nullptr;
        uint64_t initialValue = 0;
    };

    static constexpr uint64_t FENCE_WAIT_INFINITE = 1'000'000'000;

    typedef struct FenceSync
    {
        const struct Fence* fence = nullptr;
        uint64_t value = 0;
        PipelineStageFlags stages = PipelineStages::AllCommands;
    } FenceWait, FenceSignal;

    struct Fence : RefCounted<Fence>
    {
        Fence() = default;
        ~Fence() override = default;

        virtual bool initialize(const FenceDesc& fenceDesc) = 0;
        virtual void destroy() = 0;

        virtual uint64_t getCompletedValue() const = 0;
        virtual void signalOnCPU(uint64_t value) = 0;
        virtual bool waitOnCPU(uint64_t value, uint64_t timeoutNs = FENCE_WAIT_INFINITE) = 0;
        virtual bool isComplete(uint64_t value) const { return getCompletedValue() >= value; }

        virtual void setName(StringView name) {}
    };
}
