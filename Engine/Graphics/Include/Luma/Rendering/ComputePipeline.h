#pragma once
#include "Luma/Memory/RefCounted.h"

namespace Luma
{
    struct IShaderProgram;
    struct IGpuDevice;

    struct FComputePipelineDesc
    {
        IGpuDevice* device = nullptr;
        IShaderProgram* computeShader = nullptr;
    };
    
    struct IComputePipeline : IRefCounted<IComputePipeline>
    {
        ~IComputePipeline() override = default;

        virtual bool initialize(const FComputePipelineDesc& pipelineDesc) = 0;
        virtual void destroy() = 0;
    };
}