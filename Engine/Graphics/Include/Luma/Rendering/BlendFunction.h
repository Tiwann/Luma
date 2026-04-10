#pragma once
#include "BlendFactor.h"
#include "BlendOperation.h"

namespace luma
{
    struct FBlendFunction
    {
        EBlendFactor colorSource;
        EBlendFactor colorDest;
        EBlendOperation colorOp;
        EBlendFactor alphaSource;
        EBlendFactor alphaDest;
        EBlendOperation alphaOp;

        static constexpr const FBlendFunction& AlphaBlend()
        {
            static FBlendFunction alphaBlend = {
                EBlendFactor::SourceAlpha,
                EBlendFactor::OneMinusSourceAlpha,
                EBlendOperation::Add,
                EBlendFactor::One,
                EBlendFactor::Zero,
                EBlendOperation::Add
            };
            return alphaBlend;
        }

        static constexpr const FBlendFunction& AdditiveBlend()
        {
            static FBlendFunction additiveBlend = {
                EBlendFactor::SourceAlpha,
                EBlendFactor::OneMinusSourceAlpha,
                EBlendOperation::Add,
                EBlendFactor::One,
                EBlendFactor::Zero,
                EBlendOperation::Add
            };
            return additiveBlend;
        }
    };

    constexpr FBlendFunction MakeBlendFunction(EBlendFactor src, EBlendFactor dest, EBlendOperation op)
    {
        return { src, dest, op, src, dest, op };
    }
}
