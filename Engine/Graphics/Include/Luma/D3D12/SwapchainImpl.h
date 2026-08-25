#pragma once
#include "Luma/Rendering/Swapchain.h"
#include "Luma/D3D12/D3D12Fwd.h"

namespace Luma::D3D12
{
    class FSwapchainImpl final : public ISwapchain
    {
    public:
        bool initialize(const FSwapchainDesc& swapchainDesc) override;
        void destroy() override;
        bool resize(uint32_t width, uint32_t height) override;

        IDXGISwapChain4* getHandle() const { return m_Handle; }
        ID3D12Resource* getImage(uint32_t index) const { return m_Images[index]; }
        uintptr_t getImageView(uint32_t index) const { return m_ImageViews[index]; }
    private:
        IDXGISwapChain4* m_Handle = nullptr;
        ID3D12DescriptorHeap* m_DescriptorHeap = nullptr;
        ID3D12Resource* m_Images[3] { nullptr };
        uintptr_t m_ImageViews[3] {0};
    };
}
