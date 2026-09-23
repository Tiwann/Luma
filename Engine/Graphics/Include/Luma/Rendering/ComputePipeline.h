#pragma once
#include "Luma/Memory/RefCounted.h"

namespace Luma::RHI
{
    struct Device;
    struct Shader;

    struct ComputePipelineDesc
    {
        Device* device = nullptr;
        Shader* shaderProgram = nullptr;
    };
    
    struct ComputePipeline : RefCounted<ComputePipeline>
    {
        ~ComputePipeline() override = default;

        virtual bool initialize(const ComputePipelineDesc& pipelineDesc) = 0;
        virtual void destroy() = 0;
    };
}