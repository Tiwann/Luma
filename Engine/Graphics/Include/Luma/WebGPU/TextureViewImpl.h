#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/TextureView.h"
#include "WgpuFwd.h"

namespace Luma::WebGPU
{
    class FGPUDeviceImpl;
    class FSwapchainImpl;

    class LUMA_GRAPHICS_API FTextureViewImpl final : public ITextureView
    {
    public:
        bool initialize(const FTextureViewDesc& textureViewDesc) override;
        void destroy() override;
        void setName(FStringView name) override;
        WGPUTextureView getHandle() const;
    private:
        friend FSwapchainImpl;
        WGPUTextureView m_Handle = nullptr;
        FGPUDeviceImpl* m_Device = nullptr;
    };
}