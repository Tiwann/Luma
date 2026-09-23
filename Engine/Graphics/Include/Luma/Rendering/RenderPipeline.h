#pragma once
#include "BlendFunction.h"
#include "ColorChannel.h"
#include "CompareOperation.h"
#include "CullMode.h"
#include "FrontFace.h"
#include "InputLayout.h"
#include "PolygonMode.h"
#include "PrimitiveTopology.h"
#include "SampleCount.h"
#include "Luma/Memory/RefCounted.h"


namespace Luma::RHI
{
    struct Shader;
    struct Device;

    struct InputAssemblyState
    {
        bool primitiveRestartEnable = false;
        PrimitiveTopology topology = PrimitiveTopology::TriangleList;
    };

    struct RasterizationState
    {
        CullMode cullMode = CullMode::BackFace;
        FrontFace frontFace = FrontFace::CounterClockwise;
        PolygonMode polygonMode = PolygonMode::Fill;
        bool discardEnable = false;
        bool depthClampEnable = false;
        bool depthBiasEnable = false;
        float depthBiasClamp = 0.0f;
        float depthBiasConstantFactor = 0.0f;
        float depthBiasSlopeFactor = 0.0f;
        float lineWidth = 1.0f;
    };

    struct ColorBlendState
    {
        bool colorBlendEnable = false;
        BlendFunction blendFunction = BlendFunction::alphaBlend();
        ColorChannelFlags colorWriteMask = ColorChannels::All;

        static constexpr const ColorBlendState& alphaBlend()
        {
            static ColorBlendState state;
            state.colorBlendEnable = true;
            state.blendFunction = BlendFunction::alphaBlend();
            state.colorWriteMask = ColorChannels::All;
            return state;
        }

        static constexpr const ColorBlendState& disabled()
        {
            static ColorBlendState state;
            state.colorBlendEnable = false;
            return state;
        }
    };

    struct DepthStencilState
    {
        bool depthTestEnable = false;
        bool depthWriteEnable = false;
        bool stencilTestEnable = false;
        CompareOp depthCompareOp = CompareOp::Less;
    };

    struct MultisampleState
    {
        SampleCount sampleCount = SampleCount1x;
        bool alphaToCoverageEnable = false;
        bool alphaToOneEnable = false;
        bool sampleShadingEnable = false;
    };

    struct ViewportState
    {
        uint32_t x = 0;
        uint32_t y = 0;
        uint32_t width = 0;
        uint32_t height = 0;
        float minDepth = 0.0f;
        float maxDepth = 1.0f;
    };

    struct ScissorState
    {
        uint32_t x = 0;
        uint32_t y = 0;
        uint32_t width = 0;
        uint32_t height = 0;
    };

    struct RenderPipelineDesc
    {
        Device* device = nullptr;
        Shader* shaderProgram = nullptr;
        InputAssemblyState inputAssembly{};
        VertexInputLayout inputLayout{};
        RasterizationState rasterization{};
        MultisampleState multisample{};
        uint32_t colorTargetCount = 0;
        Format colorFormats[8]{Format::None};
        ColorBlendState colorBlend[8]{ColorBlendState::disabled()};
        Format depthFormat = Format::None;
        DepthStencilState depthStencil;
    };

    struct RenderPipeline : RefCounted<RenderPipeline>
    {
        ~RenderPipeline() override = default;

        virtual bool initialize(const RenderPipelineDesc& pipelineDesc) = 0;
        virtual void destroy() = 0;
    };
}
