#include "Luma/D3D12/FenceImpl.h"
#include "Luma/D3D12/GpuDeviceImpl.h"

#include <directx/d3d12.h>

namespace Luma::D3D12
{
    bool FFenceImpl::initialize(const FFenceDesc& fenceDesc)
    {
        FGpuDeviceImpl* device = static_cast<FGpuDeviceImpl*>(fenceDesc.device);
        ID3D12Device15* deviceHandle = device->getHandle();

        if (DX_FAILED(deviceHandle->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Handle))))
            return false;

        m_Device = device;
        return true;
    }

    void FFenceImpl::destroy()
    {
        if (m_Handle) m_Handle->Release();
        m_Device = nullptr;
        m_Handle = nullptr;
    }

    void FFenceImpl::wait(const uint64_t timeoutNs)
    {

    }

    void FFenceImpl::reset()
    {

    }

    void FFenceImpl::setName(FStringView name)
    {

    }
}
