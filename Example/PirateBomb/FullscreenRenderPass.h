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

using Luma::Ref;
using Luma::RHI::Device;
using Luma::RHI::Texture;
using Luma::RHI::Shader;
using Luma::RHI::Sampler;
using Luma::RHI::RenderPipeline;
using Luma::RHI::BindingGroup;
using Luma::RHI::CommandBuffer;

class FullscreenRenderPass
{
public:
    FullscreenRenderPass(Ref<Device> device, Luma::Format format);
    ~FullscreenRenderPass();

    void setInputTexture(const Texture* texture);
    void setOutputTexture(const Texture* texture);
    void setSize(const Luma::FVector2u& size);
    void execute(CommandBuffer* cmdBuffer);

private:
    Ref<Shader> fullscreenShader = nullptr;
    Ref<RenderPipeline> fullscreenPipeline = nullptr;
    Ref<Sampler> fullscreenSampler = nullptr;
    Ref<BindingGroup> fullscreenBindingGroup = nullptr;
    const Texture* inputTexture = nullptr;
    const Texture* outputTexture = nullptr;
    Luma::FVector2u m_Size;
};
