#pragma once
#include "Luma/Memory/Ref.h"
#include "Luma/Rendering/GPUDevice.h"
#include "Luma/Rendering/CommandBuffer.h"
#include "Luma/Rendering/RenderPipeline.h"
#include "Luma/Rendering/Sampler.h"
#include "Luma/Rendering/Texture.h"
#include "Luma/Rendering/Shader.h"
#include "Luma/Rendering/BindingGroup.h"
#include "Luma/Math/Vector2.h"

class FullscreenRenderPass
{
public:
    FullscreenRenderPass(Luma::Ref<Luma::IGPUDevice> device, Luma::EFormat format);
    ~FullscreenRenderPass();

    void setInputTexture(const Luma::ITexture* texture);
    void setOutputTexture(const Luma::ITexture* texture);
    void setSize(const Luma::FVector2u& size);
    void execute(Luma::ICommandBuffer* cmdBuffer);

private:
    Luma::Ref<Luma::IShader> fullscreenShader = nullptr;
    Luma::Ref<Luma::IRenderPipeline> fullscreenPipeline = nullptr;
    Luma::Ref<Luma::ISampler> fullscreenSampler = nullptr;
    Luma::Ref<Luma::IBindingGroup> fullscreenBindingGroup = nullptr;
    const Luma::ITexture* inputTexture = nullptr;
    const Luma::ITexture* outputTexture = nullptr;
    Luma::FVector2u m_Size;
};
