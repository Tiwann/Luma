#pragma once
#include "Luma/Rendering/Swapchain.h"
#include "Luma/Graphics/Export.h"
#include "DekoFwd.h"

namespace Luma
{
    struct IFence;
}

namespace Luma::Deko3d
{
    class FGpuDeviceImpl;

    class LUMA_GRAPHICS_API FSwapchainImpl final : public ISwapchain
    {
    public:
        bool initialize(const FSwapchainDesc& swapchainDesc) override;
        void destroy() override;
        bool acquireNextImage(IFence* fence, uint32_t& frameIndex);

        DkSwapchain getHandle() const;
        const DkSwapchain* getHandlePtr() const;

        DkImage* getImage(uint32_t index) const;
        DkImageView* getImageView(uint32_t index) const;

        bool isValid() const override;
        ITexture* getTexture(uint32_t index) override;
        ITextureView* getTextureView(uint32_t index) override;

        void setName(FStringView name) override;
        void setSwapInterval(uint32_t swapInterval);
    private:
        struct Private;
        Private* m_Private = nullptr;
        FGpuDeviceImpl* m_Device = nullptr;
        DkSwapchain m_Handle = nullptr;
        bool m_IsValid = false;
    };
}
