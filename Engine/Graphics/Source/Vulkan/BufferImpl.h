#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/Buffer.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class FRenderDeviceImpl;

    class LUMA_GRAPHICS_API FBufferImpl final : public IBuffer
    {
    public:
        FBufferImpl() = default;
        ~FBufferImpl() override = default;

        bool initialize(const FBufferDesc& bufferDesc) override;
        void destroy() override;

        void* map() override;
        void unmap(const void* ptr) override;

        VkBuffer getHandle() const;
        const VkBuffer* getHandlePtr() const;
        void getAllocationInfo(VmaAllocationInfo2& outAllocationInfo) const;
    private:
        FRenderDeviceImpl* m_Device = nullptr;
        VkBuffer m_Handle = nullptr;
        VmaAllocation m_Allocation = nullptr;
    };
}
