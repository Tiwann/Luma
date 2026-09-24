#pragma once
#include <functional>

namespace Luma::RHI
{
    struct CommandBuffer;
    struct Device;
    struct Queue;

    struct ImmediateExecutor
    {
        ImmediateExecutor() = default;
        virtual ~ImmediateExecutor() = default;
        virtual bool initialize(Device* device, Queue* queue) = 0;
        virtual void destroy() = 0;

        virtual void execute(const std::function<void(CommandBuffer* cmdBuffer)>& function) = 0;
        virtual bool isValid() const = 0;
    };
}