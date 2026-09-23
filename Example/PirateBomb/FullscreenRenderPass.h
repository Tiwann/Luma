#pragma once
#include "Luma/Memory/Ref.h"
#include "Luma/Rendering/Device.h"
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
    FullscreenRenderPass(Luma::Ref<Luma::Device> device, Luma::Format format);
    ~FullscreenRenderPass();

    void setInputTexture(const Luma::Texture* texture);
    void setOutputTexture(const Luma::Texture* texture);
    void setSize(const Luma::FVector2u& size);
    void execute(Luma::CommandBuffer* cmdBuffer);

private:
    Luma::Ref<Luma::Shader> fullscreenShader = nullptr;
    Luma::Ref<Luma::RenderPipeline> fullscreenPipeline = nullptr;
    Luma::Ref<Luma::Sampler> fullscreenSampler = nullptr;
    Luma::Ref<Luma::BindingGroup> fullscreenBindingGroup = nullptr;
    const Luma::Texture* inputTexture = nullptr;
    const Luma::Texture* outputTexture = nullptr;
    Luma::FVector2u m_Size;
};
