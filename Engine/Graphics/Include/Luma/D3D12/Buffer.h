#pragma once
#include "Luma/Rendering/Buffer.h"
#include "D3D12Fwd.h"

namespace Luma::D3D12
{
    class FGpuDeviceImpl;

    class FBufferImpl final : public IBuffer
    {
    public:
        bool initialize(const FBufferDesc& bufferDesc) override;
        void destroy() override;
        void* map() override;
        void unmap(const void* ptr) override;
        uint64_t getDeviceAddress() const override;

        ID3D12Resource* getHandle() const { return m_Handle; }
        ID3D12Allocation* getAllocation() const { return m_Allocation; }
    private:
        FGpuDeviceImpl* m_Device = nullptr;
        ID3D12Allocation* m_Allocation = nullptr;
        ID3D12Resource* m_Handle = nullptr;
    };
}
