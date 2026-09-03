#include "Luma/D3D12/ImguiRendererImpl.h"
#include "Luma/D3D12/GpuDeviceImpl.h"
#include "Luma/Runtime/DesktopWindow.h"

#define RGFW_WINDOWS
#define RGFW_VULKAN
#define RGFW_IMPLEMENTATION
#include <rgfw/rgfw.h>
#include <imgui_impl_dx12.h>
#define RGFW_IMGUI_IMPLEMENTATION
#include <rgfw/imgui_impl_rgfw.h>

#include <directx/d3d12.h>

namespace Luma::D3D12
{
    bool FImguiRendererImpl::initialize(const FImguiRendererDesc& rendererDesc)
    {
        if (!IImguiRenderer::initialize(rendererDesc)) return false;

        if (FDesktopWindow* desktopWindow = dynamic_cast<FDesktopWindow*>(rendererDesc.window))
        {
            ImGui_ImplRgfw_InitForD3D12(desktopWindow->getHandle(), true);
        }

        FGpuDeviceImpl* device = static_cast<FGpuDeviceImpl*>(rendererDesc.device);
        const FSwapchainImpl* swapchain = static_cast<FSwapchainImpl*>(device->getSwapchain());
        const FQueueImpl* renderQueue = static_cast<FQueueImpl*>(device->getRenderQueue());

        ImGui_ImplDX12_InitInfo initInfo{};
        initInfo.CommandQueue = renderQueue->getHandle();
        initInfo.Device = reinterpret_cast<ID3D12Device*>(device->getHandle());


        if (!ImGui_ImplDX12_Init(&initInfo))
            return false;

        return true;
    }

    void FImguiRendererImpl::destroy()
    {
    }

    void FImguiRendererImpl::beginFrame()
    {
    }

    void FImguiRendererImpl::endFrame()
    {
    }

    void FImguiRendererImpl::render(ICommandBuffer* cmdBuffer)
    {
    }

    void FImguiRendererImpl::drawTexture(const ITextureView* textureView, const FVector2f& uv0, const FVector2f& uv1)
    {
    }
}
