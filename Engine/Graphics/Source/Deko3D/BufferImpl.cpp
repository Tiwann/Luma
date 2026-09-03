#include "Luma/Deko3D/BufferImpl.h"
#include "Luma/Deko3D/GpuDeviceImpl.h"
#include "Luma/Deko3D/Conversions.h"
#include <deko3d.h>

namespace Luma::Deko3d
{
    bool FBufferImpl::initialize(const FBufferDesc& bufferDesc)
    {
        dkMemBlockDestroy(m_Handle);

        if (!bufferDesc.device) return false;
        if (bufferDesc.usage == EBufferUsage::None) return false;
        if (bufferDesc.size == 0) return false;

        FGpuDeviceImpl* device = static_cast<FGpuDeviceImpl*>(bufferDesc.device);
        const DkDevice deviceHandle = device->getHandle();

        DkMemBlockMaker maker;
        dkMemBlockMakerDefaults(&maker, deviceHandle, bufferDesc.size);
        maker.flags = convert<uint32_t>(bufferDesc.usage);

        m_Handle = dkMemBlockCreate(&maker);
        if (bufferDesc.alwaysMapped)
            m_MappedAddress = dkMemBlockGetCpuAddr(m_Handle);

        m_Size = bufferDesc.size;
        m_State = EResourceState::General;
        m_Usage = bufferDesc.usage;
        m_AlwaysMapped = bufferDesc.alwaysMapped;
        m_Device = device;
        return true;
    }

    void FBufferImpl::destroy()
    {
        if (m_Handle)
        {
            dkMemBlockDestroy(m_Handle);
            m_Handle = nullptr;
        }
    }

    void* FBufferImpl::map()
    {
        if (m_AlwaysMapped) return m_MappedAddress;
        return dkMemBlockGetCpuAddr(m_Handle);
    }

    void FBufferImpl::unmap(const void* ptr)
    {
        (void)ptr;
    }

    uint64_t FBufferImpl::getDeviceAddress() const
    {
        if (!m_Handle) return 0;
        return dkMemBlockGetGpuAddr(m_Handle);
    }
}
