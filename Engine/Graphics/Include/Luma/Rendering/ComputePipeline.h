#pragma once
#include "Luma/Memory/RefCounted.h"

namespace Luma
{
    struct IShader;
    struct IGPUDevice;

    struct FComputePipelineDesc
    {
        IGPUDevice* device = nullptr;
        IShader* shaderProgram = nullptr;
    };
    
    struct IComputePipeline : IRefCounted<IComputePipeline>
    {
        ~IComputePipeline() override = default;

        virtual bool initialize(const FComputePipelineDesc& pipelineDesc) = 0;
        virtual void destroy() = 0;
    };
}