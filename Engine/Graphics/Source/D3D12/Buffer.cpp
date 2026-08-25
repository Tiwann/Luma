#include "Luma/D3D12/Buffer.h"
#include "Luma/D3D12/RenderDeviceImpl.h"
#include <D3D12MemAlloc.h>
#include <directx/d3d12.h>

#include "Luma/Containers/StringConversion.h"

namespace Luma::D3D12
{
    bool FBufferImpl::initialize(const FBufferDesc& bufferDesc)
    {
        if (!bufferDesc.device) return false;
        if (bufferDesc.size == 0) return false;

        if (m_Handle) m_Handle->Release();
        if (m_Allocation) m_Allocation->Release();

        const D3D12_HEAP_TYPE heapType = bufferDesc.usage == EBufferUsage::StagingBuffer
            ? D3D12_HEAP_TYPE_UPLOAD : D3D12_HEAP_TYPE_DEFAULT;
        const CALLOCATION_DESC allocDesc = CALLOCATION_DESC{ heapType };
        
        D3D12_RESOURCE_DESC resourceDesc = {};
        resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        resourceDesc.Alignment = 0;
        resourceDesc.Width = bufferDesc.size;
        resourceDesc.Height = 1;
        resourceDesc.DepthOrArraySize = 1;
        resourceDesc.MipLevels = 1;
        resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        resourceDesc.SampleDesc.Count = 1;
        resourceDesc.SampleDesc.Quality = 0;
        resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

        FRenderDeviceImpl* device = static_cast<FRenderDeviceImpl*>(bufferDesc.device);
        ID3D12Allocator* allocator = device->getAllocator();
        if (!allocator) return false;

        ID3D12Resource* buffer = nullptr;
        ID3D12Allocation* allocation = nullptr;

        D3D12_RESOURCE_STATES initialState =
            (heapType == D3D12_HEAP_TYPE_UPLOAD)
                ? D3D12_RESOURCE_STATE_GENERIC_READ
                : D3D12_RESOURCE_STATE_COMMON;

        if (DX_FAILED(allocator->CreateResource(
            &allocDesc,
            &resourceDesc,
            initialState,
            nullptr,
            &allocation,
            IID_PPV_ARGS(&buffer))))
        return false;

        if (bufferDesc.alwaysMapped)
        {
            const D3D12_RANGE& range = D3D12_RANGE(0, 0);
            (void)buffer->Map(0, &range, &m_MappedAddress);
        }

        if (!bufferDesc.debugName.isEmpty())
        {
            FWideString debugName = stringConvert<wchar_t, char>(bufferDesc.debugName);
            (void)buffer->SetName(*debugName);
        }

        m_Allocation = allocation;
        m_Handle = buffer;
        m_Device = device;
        m_Size = bufferDesc.size;
        m_Usage = bufferDesc.usage;
        m_AlwaysMapped = bufferDesc.alwaysMapped;
        m_MappedAddress = (m_AlwaysMapped && heapType == D3D12_HEAP_TYPE_UPLOAD) ? map() : nullptr;
        m_State = initialState == D3D12_RESOURCE_STATE_GENERIC_READ ? EResourceState::CopySource : EResourceState::General;
        m_AlwaysMapped = bufferDesc.alwaysMapped;
        return true;
    }

    void FBufferImpl::destroy()
    {
        if (m_Handle)
        {
            m_Handle->Release();
            m_Handle = nullptr;
        }

        if (m_Allocation)
        {
            m_Allocation->Release();
            m_Allocation = nullptr;
        }
    }

    void* FBufferImpl::map()
    {
        if (m_AlwaysMapped) return m_MappedAddress;

        void* mapped = nullptr;
        const D3D12_RANGE& range = D3D12_RANGE(0, 0);
        if (DX_FAILED(m_Handle->Map(0, &range, &mapped)))
            return nullptr;
        return mapped;
    }

    void FBufferImpl::unmap(const void* ptr)
    {
        if (m_AlwaysMapped) return;
        (void)ptr;
        const D3D12_RANGE& range = D3D12_RANGE(0, m_Size);
        m_Handle->Unmap(0, &range);
    }

    uint64_t FBufferImpl::getDeviceAddress() const
    {
        if (!m_Handle) return 0;
        return m_Handle->GetGPUVirtualAddress();
    }
}
