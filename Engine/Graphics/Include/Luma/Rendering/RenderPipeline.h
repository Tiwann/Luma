#pragma once
#include "ColorChannel.h"
#include "CullMode.h"
#include "FrontFace.h"
#include "InputLayout.h"
#include "PolygonMode.h"
#include "PrimitiveTopology.h"
#include "BlendFunction.h"
#include "CompareOperation.h"
#include "SampleCount.h"
#include "Luma/Memory/RefCounted.h"

namespace Luma
{
    struct IRenderDevice;
    struct IShaderProgram;

    struct FInputAssemblyState
    {
        bool primitiveRestartEnable = false;
        EPrimitiveTopology topology = EPrimitiveTopology::TriangleList;
    };

    struct FRasterizationState
    {
        ECullMode cullMode = ECullMode::BackFace;
        EFrontFace frontFace = EFrontFace::CounterClockwise;
        EPolygonMode polygonMode = EPolygonMode::Fill;
        bool discardEnable = false;
        bool depthClampEnable = false;
        bool depthBiasEnable = false;
        float depthBiasClamp = 0.0f;
        float depthBiasConstantFactor = 0.0f;
        float depthBiasSlopeFactor = 0.0f;
        float lineWidth = 1.0f;
    };

    struct FColorBlendState
    {
        bool colorBlendEnable = false;
        FBlendFunction blendFunction = FBlendFunction::alphaBlend();
        FColorChannelFlags colorWriteMask = EColorChannelBits::Red | EColorChannelBits::Green | EColorChannelBits::Blue | EColorChannelBits::Alpha;
    };

    struct FDepthStencilState
    {
        bool depthTestEnable = false;
        bool depthWriteEnable = false;
        bool stencilTestEnable = false;
        ECompareOp depthCompareOp = ECompareOp::Less;
    };

    struct FMultisampleState
    {
        ESampleCount sampleCount = ESampleCount::SampleCount1x;
        bool alphaToCoverageEnable = false;
        bool alphaToOneEnable = false;
        bool sampleShadingEnable = false;
    };

    struct FViewportState
    {
        uint32_t x = 0;
        uint32_t y = 0;
        uint32_t width = 0;
        uint32_t height = 0;
        float minDepth = 0.0f;
        float maxDepth = 1.0f;
    };

    struct FScissorState
    {
        uint32_t x = 0;
        uint32_t y = 0;
        uint32_t width = 0;
        uint32_t height = 0;
    };

    struct FRenderPipelineDesc
    {
        IRenderDevice* device = nullptr;
        const IShaderProgram* vertexShader = nullptr;
        const IShaderProgram* tessellationControlShader = nullptr;
        const IShaderProgram* tessellationEvaluationShader = nullptr;
        const IShaderProgram* geometryShader = nullptr;
        const IShaderProgram* fragmentShader = nullptr;
        const IShaderProgram* amplificationShader = nullptr;
        FInputAssemblyState inputAssembly{};
        FVertexInputLayout inputLayout{};
        FRasterizationState rasterization{};
        FColorBlendState colorBlend[8]{};
        FDepthStencilState depthStencil{};
        FMultisampleState multisample{};
        EFormat colorFormats[8] { EFormat::None };
        uint32_t colorFormatCount = 0;
        EFormat depthFormat = EFormat::None;
    };

    struct IRenderPipeline : IRefCounted<IRenderPipeline>
    {
        ~IRenderPipeline() override = default;

        virtual bool initialize(const FRenderPipelineDesc& pipelineDesc) = 0;
        virtual void destroy() = 0;
    };
}
