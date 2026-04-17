#pragma once
#include "BlendFactor.h"
#include "BlendOperation.h"

namespace Luma
{
    struct FBlendFunction
    {
        EBlendFactor colorSource;
        EBlendFactor colorDest;
        EBlendOp colorOp;
        EBlendFactor alphaSource;
        EBlendFactor alphaDest;
        EBlendOp alphaOp;

        static constexpr const FBlendFunction& AlphaBlend()
        {
            static FBlendFunction alphaBlend = {
                EBlendFactor::SourceAlpha,
                EBlendFactor::OneMinusSourceAlpha,
                EBlendOp::Add,
                EBlendFactor::One,
                EBlendFactor::Zero,
                EBlendOp::Add
            };
            return alphaBlend;
        }

        static constexpr const FBlendFunction& AdditiveBlend()
        {
            static FBlendFunction additiveBlend = {
                EBlendFactor::SourceAlpha,
                EBlendFactor::OneMinusSourceAlpha,
                EBlendOp::Add,
                EBlendFactor::One,
                EBlendFactor::Zero,
                EBlendOp::Add
            };
            return additiveBlend;
        }
    };

    constexpr FBlendFunction MakeBlendFunction(EBlendFactor src, EBlendFactor dest, EBlendOp op)
    {
        return { src, dest, op, src, dest, op };
    }
}
