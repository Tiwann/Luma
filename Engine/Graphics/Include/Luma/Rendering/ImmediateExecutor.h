#pragma once
#include "Luma/Memory/Ref.h"
#include <functional>

namespace Luma::RHI
{
    struct CommandBuffer;
    struct Device;
    struct Queue;
    struct Fence;

    class ImmediateExecutor
    {
    public:
        ImmediateExecutor(Device* device, Queue* queue);
        ~ImmediateExecutor();
        void execute(const std::function<void(CommandBuffer* cmdBuffer)>& function);
        bool isValid() const;

    private:
        bool m_IsValid = false;
        Device* m_Device = nullptr;
        Queue* m_Queue = nullptr;
        Ref<CommandBuffer> m_CmdBuf = nullptr;
        Ref<Fence> m_Fence = nullptr;
        uint32_t m_FenceValue = 0;
    };
}
