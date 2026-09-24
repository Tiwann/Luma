#include "FullscreenRenderPass.h"
#include "Luma/Rendering/RenderPassDesc.h"
#include "Luma/Runtime/Path.h"

using namespace Luma;
using namespace Luma::RHI;

FullscreenRenderPass::FullscreenRenderPass(Ref<Device> device, Format format)
{
    const String fullscreenVertexPath = Path::getEngineShaderPath("Fullscreen.slang.vert.spv");
    const String fullscreenFragmentPath = Path::getEngineShaderPath("Fullscreen.slang.frag.spv");
    fullscreenShader = device->createShader(fullscreenVertexPath, fullscreenFragmentPath);

    RenderPipelineDesc fullscreenPipelineDesc;
    fullscreenPipelineDesc.shaderProgram = fullscreenShader;
    fullscreenPipelineDesc.colorFormats[0] = format;
    fullscreenPipelineDesc.colorTargetCount = 1;
    fullscreenPipeline = device->createRenderPipeline(fullscreenPipelineDesc);

    fullscreenSampler = device->createSampler(SamplerDesc());
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

void FullscreenRenderPass::setInputTexture(const Texture* texture)
{
    inputTexture = texture;
    fullscreenBindingGroup->bindTextureWithSampler(0, inputTexture, fullscreenSampler);
}

void FullscreenRenderPass::setOutputTexture(const Texture* texture)
{
    outputTexture = texture;
}

void FullscreenRenderPass::setSize(const FVector2u& size)
{
    m_Size = size;
}

void FullscreenRenderPass::execute(CommandBuffer* cmdBuffer)
{
    RenderPassTarget renderTarget;
    renderTarget.type = RenderPassTargetType::Color;
    renderTarget.loadOp = LoadOp::Clear;
    renderTarget.storeOp = StoreOp::Store;
    renderTarget.textureView = outputTexture->getTextureView();

    RenderPassDesc renderPassDesc;
    renderPassDesc.renderArea = {0, 0, m_Size.x, m_Size.y};
    renderPassDesc.colorTargets.add(&renderTarget);

    cmdBuffer->beginRenderPass(renderPassDesc);
    cmdBuffer->bindBindingGroup(fullscreenBindingGroup);
    cmdBuffer->bindRenderPipeline(fullscreenPipeline);
    cmdBuffer->setViewport(Viewport::fromSize(m_Size.x, m_Size.y));
    cmdBuffer->setScissor(Scissor::fromSize(m_Size.x, m_Size.y));
    cmdBuffer->draw(6, 1, 0, 0);
    cmdBuffer->endRenderPass();
}
