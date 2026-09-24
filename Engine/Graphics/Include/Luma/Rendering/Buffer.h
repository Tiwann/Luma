#pragma once
#include <cstdint>
#include "BufferUsage.h"
#include "Resource.h"
#include "Luma/Containers/StringView.h"
#include "Luma/Memory/RefCounted.h"

namespace Luma::RHI
{
    struct Device;

    struct BufferDesc
    {
        Device* device = nullptr;
        BufferUsage usage = BufferUsage::None;
        uint64_t size = 0;
        bool alwaysMapped = false;
        String debugName;
    };

    struct Buffer : Resource, RefCounted<Buffer>
    {
        Buffer() = default;
        ~Buffer() override = default;

        virtual bool initialize(const BufferDesc& bufferDesc) = 0;
        virtual void destroy() = 0;

        virtual void* map() = 0;
        virtual void unmap(const void* ptr) = 0;
        virtual uint64_t getDeviceAddress() const = 0;

        template<typename T>
        T* map() { return static_cast<T*>(map()); }

        ResourceType getResourceType() const final { return ResourceType::Buffer; }
        ResourceState getResourceState() const final { return m_State; }
        void setResourceState(const ResourceState state) { m_State = state; }
        BufferUsage getUsage() const { return m_Usage; }
        uint64_t getSize() const { return m_Size; }
        bool isAlwaysMapped() const { return m_AlwaysMapped; }

        virtual void setName(StringView name){}
    protected:
        uint64_t m_Size = 0;
        BufferUsage m_Usage = BufferUsage::None;
        ResourceState m_State = ResourceState::Undefined;
        bool m_AlwaysMapped = false;
        void* m_MappedAddress = nullptr;
    };
}
