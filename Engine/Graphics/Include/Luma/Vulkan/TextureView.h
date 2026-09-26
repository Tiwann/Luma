#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Containers/StringView.h"
#include "Luma/Rendering/TextureView.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class Device;
    class Swapchain;

    class LUMA_GRAPHICS_API TextureView final : public RHI::TextureView
    {
    public:
        bool initialize(const RHI::TextureViewDesc& textureViewDesc) override;
        void destroy() override;
        void setName(StringView name) override;
        VkImage getImage() const;
        VkImageView getHandle() const;
    private:
        friend Swapchain;
        VkImageView m_Handle = nullptr;
        Device* m_Device = nullptr;
    };
}
