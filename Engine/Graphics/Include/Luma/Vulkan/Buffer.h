#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/Buffer.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class Device;

    class LUMA_GRAPHICS_API Buffer final : public RHI::Buffer
    {
    public:
        Buffer() = default;
        ~Buffer() override = default;

        bool initialize(const RHI::BufferDesc& bufferDesc) override;
        void destroy() override;

        void* map() override;
        void unmap(const void* ptr) override;
        uint64_t getDeviceAddress() const override;

        VkBuffer getHandle() const;
        const VkBuffer* getHandlePtr() const;
        void getAllocationInfo(VmaAllocationInfo2& outAllocationInfo) const;

        void setName(StringView name) override;
    private:
        Device* m_Device = nullptr;
        VkBuffer m_Handle = nullptr;
        VmaAllocation m_Allocation = nullptr;
    };
}
