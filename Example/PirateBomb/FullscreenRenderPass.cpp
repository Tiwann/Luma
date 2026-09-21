#include "FullscreenRenderPass.h"
#include "Luma/Rendering/RenderPassDesc.h"
#include "Luma/Runtime/Path.h"

FullscreenRenderPass::FullscreenRenderPass(Luma::Ref<Luma::IGPUDevice> device, Luma::EFormat format)
{
    const Luma::FString fullscreenVertexPath = Luma::FPath::getEngineShaderPath("Fullscreen.slang.vert.spv");
    const Luma::FString fullscreenFragmentPath = Luma::FPath::getEngineShaderPath("Fullscreen.slang.frag.spv");
    fullscreenShader = device->createShader(fullscreenVertexPath, fullscreenFragmentPath);

    Luma::FRenderPipelineDesc fullscreenPipelineDesc;
    fullscreenPipelineDesc.shaderProgram = fullscreenShader;
    fullscreenPipelineDesc.colorFormats[0] = format;
    fullscreenPipelineDesc.colorTargetCount = 1;
    fullscreenPipeline = device->createRenderPipeline(fullscreenPipelineDesc);

    fullscreenSampler = device->createSampler(Luma::FSamplerDesc());
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

void FullscreenRenderPass::setInputTexture(const Luma::ITexture* texture)
{
    inputTexture = texture;
    fullscreenBindingGroup->bindTextureWithSampler(0, inputTexture, fullscreenSampler);
}

void FullscreenRenderPass::setOutputTexture(const Luma::ITexture* texture)
{
    outputTexture = texture;
}

void FullscreenRenderPass::setSize(const Luma::FVector2u& size)
{
    m_Size = size;
}

void FullscreenRenderPass::execute(Luma::ICommandBuffer* cmdBuffer)
{
    Luma::FRenderPassTarget renderTarget;
    renderTarget.type = Luma::ERenderPassTargetType::Color;
    renderTarget.loadOp = Luma::ELoadOp::Clear;
    renderTarget.storeOp = Luma::EStoreOp::Store;
    renderTarget.textureView = outputTexture->getTextureView();

    Luma::FRenderPassDesc renderPassDesc;
    renderPassDesc.renderArea = {0, 0, m_Size.x, m_Size.y};
    renderPassDesc.colorTargets.add(&renderTarget);

    cmdBuffer->beginRenderPass(renderPassDesc);
    cmdBuffer->bindBindingGroup(fullscreenBindingGroup);
    cmdBuffer->bindRenderPipeline(fullscreenPipeline);
    cmdBuffer->setViewport(Luma::FViewport::fromSize(m_Size.x, m_Size.y));
    cmdBuffer->setScissor(Luma::FScissor::fromSize(m_Size.x, m_Size.y));
    cmdBuffer->draw(6, 1, 0, 0);
    cmdBuffer->endRenderPass();
}
