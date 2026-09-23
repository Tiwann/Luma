#include "FullscreenRenderPass.h"
#include "Luma/Rendering/RenderPassDesc.h"
#include "Luma/Runtime/Path.h"

FullscreenRenderPass::FullscreenRenderPass(Luma::Ref<Luma::Device> device, Luma::Format format)
{
    const Luma::FString fullscreenVertexPath = Luma::FPath::getEngineShaderPath("Fullscreen.slang.vert.spv");
    const Luma::FString fullscreenFragmentPath = Luma::FPath::getEngineShaderPath("Fullscreen.slang.frag.spv");
    fullscreenShader = device->createShader(fullscreenVertexPath, fullscreenFragmentPath);

    Luma::RenderPipelineDesc fullscreenPipelineDesc;
    fullscreenPipelineDesc.shaderProgram = fullscreenShader;
    fullscreenPipelineDesc.colorFormats[0] = format;
    fullscreenPipelineDesc.colorTargetCount = 1;
    fullscreenPipeline = device->createRenderPipeline(fullscreenPipelineDesc);

    fullscreenSampler = device->createSampler(Luma::SamplerDesc());
    fullscreenBindingGroup = fullscreenShader->createBindingGroup(0);
}

FullscreenRenderPass::~FullscreenRenderPass()
{
    outputTexture = nullptr;
    inputTexture = nullptr;
    fullscreenBindingGroup = nullptr;
    fullscreenSampler = nullptr;
    fullscreenPipeline = nullptr;
    fullscreenShader = nullptr;
}

void FullscreenRenderPass::setInputTexture(const Luma::Texture* texture)
{
    inputTexture = texture;
    fullscreenBindingGroup->bindTextureWithSampler(0, inputTexture, fullscreenSampler);
}

void FullscreenRenderPass::setOutputTexture(const Luma::Texture* texture)
{
    outputTexture = texture;
}

void FullscreenRenderPass::setSize(const Luma::FVector2u& size)
{
    m_Size = size;
}

void FullscreenRenderPass::execute(Luma::CommandBuffer* cmdBuffer)
{
    Luma::RenderPassTarget renderTarget;
    renderTarget.type = Luma::RenderPassTargetType::Color;
    renderTarget.loadOp = Luma::LoadOp::Clear;
    renderTarget.storeOp = Luma::StoreOp::Store;
    renderTarget.textureView = outputTexture->getTextureView();

    Luma::RenderPassDesc renderPassDesc;
    renderPassDesc.renderArea = {0, 0, m_Size.x, m_Size.y};
    renderPassDesc.colorTargets.add(&renderTarget);

    cmdBuffer->beginRenderPass(renderPassDesc);
    cmdBuffer->bindBindingGroup(fullscreenBindingGroup);
    cmdBuffer->bindRenderPipeline(fullscreenPipeline);
    cmdBuffer->setViewport(Luma::Viewport::fromSize(m_Size.x, m_Size.y));
    cmdBuffer->setScissor(Luma::Scissor::fromSize(m_Size.x, m_Size.y));
    cmdBuffer->draw(6, 1, 0, 0);
    cmdBuffer->endRenderPass();
}
