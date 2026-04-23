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
        void destroy() override;
        bool isValid() override;
        void setName(FStringView name) override;
        VkImage getImage() const;
        VmaAllocation getAllocation() const;
        EResourceState getResourceState() override;
        const ITextureView* getTextureView() const override;

    private:
        friend FSwapchainImpl;
        FRenderDeviceImpl* m_Device = nullptr;
        VkImage m_Image = nullptr;
        VmaAllocation m_Allocation = nullptr;
        FTextureViewImpl m_View;
    };
}
