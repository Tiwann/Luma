#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/Fence.h"
#include "D3D12Fwd.h"

namespace Luma::D3D12
{
    class FGpuDeviceImpl;

    class LUMA_GRAPHICS_API FFenceImpl : public IFence
    {
    public:
        bool initialize(const FFenceDesc& fenceDesc) override;
        void destroy() override;
        void wait(uint64_t timeoutNs) override;
        void reset() override;
        void setName(FStringView name) override;

        ID3D12Fence1* getHandle() const { return m_Handle; }
    private:
        FGpuDeviceImpl* m_Device = nullptr;
        ID3D12Fence1* m_Handle = nullptr;
    };
}