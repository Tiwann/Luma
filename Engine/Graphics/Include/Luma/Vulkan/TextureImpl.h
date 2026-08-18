#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/Texture.h"
#include "TextureViewImpl.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class FRenderDeviceImpl;
    class FSwapchainImpl;

    class LUMA_GRAPHICS_API FTextureImpl final : public ITexture
    {
    public:
        FTextureImpl() = default;
        ~FTextureImpl() override = default;

        bool initialize(const FTextureDesc& textureDesc) override;
        bool resize(uint32_t width, uint32_t height, uint32_t depth) override;
        void destroy() override;
        bool isValid() override;
        void setName(FStringView name) override;
        VkImage getImage() const;
        VmaAllocation getAllocation() const;
        const ITextureView* getTextureView() const override;
    private:
        friend FSwapchainImpl;
        FRenderDeviceImpl* m_Device = nullptr;
        VkImage m_Image = nullptr;
        VmaAllocation m_Allocation = nullptr;
        FTextureViewImpl m_View;
    };
}
