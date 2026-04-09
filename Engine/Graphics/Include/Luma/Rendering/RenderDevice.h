#pragma once
#include "Luma/Graphics/Export.h"
#include "Interface.h"

namespace luma
{
    struct FBufferDesc;
    struct FTextureDesc;
    struct FShaderDesc;
    struct FCommandBufferDesc;
    struct FFenceDesc;
    struct FSamplerDesc;

    struct IBuffer;
    struct ITexture;
    struct IShader;
    struct ISampler;
    struct ICommandBuffer;
    struct IFence;

    enum class ERenderDeviceType
    {
        None,
#ifdef LUMA_BUILD_VULKAN
        Vulkan,
#endif
#ifdef LUMA_BUILD_D3D12
        D3D12,
#endif
#ifdef LUMA_BUILD_OPENGL
        OpenGL,
#endif
#ifdef LUMA_BUILD_WEBGPU
        WebGPU,
#endif
#if defined(LUMA_PLATFORM_SWITCH) || defined(LUMA_BUILD_NVN)
        NVN,
#endif
#if defined(LUMA_PLATFORM_SWITCH) || defined(LUMA_BUILD_DEKO3D)
        Deko3D,
#endif
    };

    struct FRenderDeviceDesc
    {
        ERenderDeviceType deviceType;
    };

    struct LUMA_GRAPHICS_API IRenderDevice : TBase<FRenderDeviceDesc>
    {
        ~IRenderDevice() override = default;
        virtual ERenderDeviceType getDeviceType() = 0;

        virtual IBuffer* createBuffer(const FBufferDesc& bufferDesc) = 0;
        virtual ITexture* createTexture(const FTextureDesc& textureDesc) = 0;
        virtual IShader* createShader(const FShaderDesc& shaderDesc) = 0;
        virtual ICommandBuffer* createCommandBuffer(const FCommandBufferDesc& commandBufferDesc) = 0;
        virtual ISampler* createSampler(const FSamplerDesc& samplerDesc) = 0;
    };

    LUMA_GRAPHICS_API IRenderDevice* createRenderDevice(const FRenderDeviceDesc& deviceDesc);
}
