#pragma once
#include "Luma/Memory/RefCounted.h"
#include "Resource.h"
#include "BufferUsage.h"
#include "Luma/Containers/StringView.h"
#include <cstdint>

namespace Luma
{
    struct IRenderDevice;

    struct FBufferDesc
    {
        IRenderDevice* device;
        EBufferUsage usage = EBufferUsage::None;
        uint64_t size = 0;
        bool alwaysMapped = false;
    };

    struct IBuffer : IResource, IRefCounted<IBuffer>
    {
        IBuffer() = default;
        ~IBuffer() override = default;

        virtual bool initialize(const FBufferDesc& bufferDesc) = 0;
        virtual void destroy() = 0;

        virtual void* map() = 0;
        virtual void unmap(const void* ptr) = 0;
        virtual uint64_t getDeviceAddress() const = 0;

        EResourceType getResourceType() final { return EResourceType::Buffer; }
        EResourceState getResourceState() final { return m_State; }
        void setResourceState(const EResourceState state) { m_State = state; }
        EBufferUsage getUsage() const { return m_Usage; }
        uint64_t getSize() const { return m_Size; }
        bool isAlwaysMapped() const { return m_AlwaysMapped; }

        virtual void setName(FStringView name){}
    protected:
        uint64_t m_Size = 0;
        EBufferUsage m_Usage = EBufferUsage::None;
        EResourceState m_State = EResourceState::Undefined;
        bool m_AlwaysMapped = false;
        void* m_MappedData = nullptr;
    };
}
