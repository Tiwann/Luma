#pragma once
#include "Luma/Memory/RefCounted.h"

namespace Luma
{
    struct IShader;
    struct IRenderDevice;

    struct FComputePipelineDesc
    {
        IRenderDevice* device = nullptr;
        IShader* shader = nullptr;
    };
    
    struct IComputePipeline : IRefCounted<IComputePipeline>
    {
        ~IComputePipeline() override = default;

        virtual bool initialize(const FComputePipelineDesc& pipelineDesc) = 0;
        virtual void destroy() = 0;
    };
}