#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/Texture.h"
#include "TextureView.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class Device;
    class Swapchain;

    class LUMA_GRAPHICS_API Texture final : public RHI::Texture
    {
    public:
        Texture() = default;
        ~Texture() override = default;

        bool initialize(const RHI::TextureDesc& textureDesc) override;
        bool resize(uint32_t width, uint32_t height, uint32_t depth) override;
        void destroy() override;
        bool isValid() override;
        void setName(StringView name) override;
        VkImage getImage() const;
        VmaAllocation getAllocation() const;
        const RHI::TextureView* getTextureView() const override;
    private:
        friend Swapchain;
        Device* m_Device = nullptr;
        VkImage m_Image = nullptr;
        VmaAllocation m_Allocation = nullptr;
        TextureView m_View;
    };
}
