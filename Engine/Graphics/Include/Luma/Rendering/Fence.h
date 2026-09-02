#pragma once
#include "PipelineStage.h"
#include "Luma/Containers/StringView.h"
#include "Luma/Memory/RefCounted.h"
#include <cstdint>


namespace Luma
{
    struct IRenderDevice;

    struct FFenceDesc
    {
        IRenderDevice* device = nullptr;
        uint64_t initialValue = 0;
    };

    static constexpr uint64_t FENCE_WAIT_INFINITE = 1'000'000'000;

    typedef struct FenceSync
    {
        const struct IFence* fence = nullptr;
        uint64_t value = 0;
        FPipelineStageFlags stages = EPipelineStageBits::AllCommands;
    } FFenceWait, FFenceSignal;

    struct IFence : IRefCounted<IFence>
    {
        IFence() = default;
        ~IFence() override = default;

        virtual bool initialize(const FFenceDesc& fenceDesc) = 0;
        virtual void destroy() = 0;

        virtual uint64_t getCompletedValue() const = 0;
        virtual void signalOnCPU(uint64_t value) = 0;
        virtual bool waitOnCPU(uint64_t value, uint64_t timeoutNs = FENCE_WAIT_INFINITE) = 0;
        virtual bool isComplete(uint64_t value) const { return getCompletedValue() >= value; }

        virtual void setName(FStringView name) {}
    };
}
