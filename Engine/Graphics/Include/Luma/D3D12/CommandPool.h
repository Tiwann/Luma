#pragma once
#include "CommandBufferImpl.h"
#include "Rendering/CommandPool.h"
#include "Rendering/QueueType.h"

struct ID3D12CommandAllocator;

namespace Nova::D3D12
{
    class GpuDevice;
    class Queue;

    class CommandPool final : public Nova::CommandPool
    {
    public:
        bool Initialize(const CommandPoolCreateInfo& createInfo) override;
        void Destroy() override;
        void Reset() override;

        ID3D12CommandAllocator* GetHandle() { return m_Handle; }
        const ID3D12CommandAllocator* GetHandle() const { return m_Handle; }


        QueueType GetQueueType() const;

        Queue* GetQueue() { return m_Queue; }
        const Queue* GetQueue() const { return m_Queue; }

        GpuDevice* GetDevice() { return m_Device; }
        const GpuDevice* GetDevice() const { return m_Device; }
        CommandBufferImpl AllocateCommandBuffer(CommandBufferLevel level);

    private:
        GpuDevice* m_Device = nullptr;
        Queue* m_Queue = nullptr;
        ID3D12CommandAllocator* m_Handle = nullptr;
    };
}
