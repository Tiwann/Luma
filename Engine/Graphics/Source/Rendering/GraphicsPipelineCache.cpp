#include "Luma/Rendering/GraphicsPipelineCache.h"
#include "Luma/Rendering/RenderDevice.h"
#include "Luma/Rendering/GraphicsPipeline.h"

namespace Luma
{
    FGraphicsPipelineCache::FGraphicsPipelineCache(Ref<IRenderDevice> renderDevice) : m_Device(renderDevice)
    {
    }

    void FGraphicsPipelineCache::destroy()
    {
        for (auto& [_, pipeline] : m_Cache)
            pipeline->destroy();
        m_Cache.clear();
    }

    Ref<IGraphicsPipeline> FGraphicsPipelineCache::getOrCreate(const FGraphicsPipelineKey& key)
    {
        if (!m_Device) return nullptr;

        if (const auto* pipeline = m_Cache.find(key))
            return pipeline->value;

        FGraphicsPipelineDesc pipelineDesc;
        pipelineDesc.device = m_Device;
        pipelineDesc.shaderProgram = key.shader;
        pipelineDesc.inputLayout = key.vertexInputLayout;

        pipelineDesc.depthStencil.depthTestEnable = true;
        pipelineDesc.depthStencil.stencilTestEnable = false;
        pipelineDesc.depthStencil.depthWriteEnable = true;
        pipelineDesc.depthStencil.depthCompareOp = ECompareOp::Less;
        pipelineDesc.depthFormat = EFormat::D32_FLOAT_S8_UINT;

        if (key.materialType == EMaterialType::Transparent)
        {
            pipelineDesc.colorBlend[0].colorBlendEnable = true;
            pipelineDesc.colorBlend[0].blendFunction = FBlendFunction::alphaBlend();
        }

        for (uint32_t i = 0; i < key.colorFormatCount; ++i)
            pipelineDesc.colorFormats[i] = key.colorFormats[i];
        pipelineDesc.colorFormatCount = key.colorFormatCount;

        Ref<IGraphicsPipeline> pipeline = m_Device->createGraphicsPipeline(pipelineDesc);
        if (!pipeline) return nullptr;

        m_Cache[key] = pipeline;
        return pipeline;
    }
}