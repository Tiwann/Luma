#pragma once
#include <functional>

namespace Luma
{
    struct FImmediateExecutorDesc
    {
        struct IRenderDevice* device = nullptr;
        struct IQueue* queue = nullptr;
    };

    struct IImmediateExecutor
    {
        IImmediateExecutor() = default;
        virtual ~IImmediateExecutor() = default;

        virtual bool initialize(const FImmediateExecutorDesc& executorDesc) = 0;
        virtual void destroy() = 0;
        virtual void execute(const std::function<void(const struct ICommandBuffer* cmdBuffer)>& function) = 0;
        virtual bool isValid() const = 0;
    };
}