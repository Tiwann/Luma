#pragma once
#include "BlendFactor.h"
#include "BlendOperation.h"

namespace Luma
{
    struct BlendFunction
    {
        BlendFactor colorSource;
        BlendFactor colorDest;
        BlendOp colorOp;
        BlendFactor alphaSource;
        BlendFactor alphaDest;
        BlendOp alphaOp;

        static constexpr const BlendFunction& alphaBlend()
        {
            static BlendFunction alphaBlend = {
                BlendFactor::SourceAlpha,
                BlendFactor::OneMinusSourceAlpha,
                BlendOp::Add,
                BlendFactor::One,
                BlendFactor::Zero,
                BlendOp::Add
            };
            return alphaBlend;
        }

        static constexpr const BlendFunction& additiveBlend()
        {
            static BlendFunction additiveBlend = {
                BlendFactor::SourceAlpha,
                BlendFactor::OneMinusSourceAlpha,
                BlendOp::Add,
                BlendFactor::One,
                BlendFactor::Zero,
                BlendOp::Add
            };
            return additiveBlend;
        }
    };

    constexpr BlendFunction makeBlendFunction(BlendFactor src, BlendFactor dest, BlendOp op)
    {
        return { src, dest, op, src, dest, op };
    }
}
