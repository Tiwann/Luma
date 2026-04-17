#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/TextureView.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class FRenderDeviceImpl;
    class FSwapchainImpl;

    class LUMA_GRAPHICS_API FTextureViewImpl final : public ITextureView
    {
    public:
        bool initialize(const FTextureViewDesc& textureViewDesc) override;
        void destroy() override;

        VkImage getImage() const;
        VkImageView getHandle() const;
    private:
        friend FSwapchainImpl;
        VkImageView m_Handle = nullptr;
        FRenderDeviceImpl* m_Device = nullptr;
    };
}