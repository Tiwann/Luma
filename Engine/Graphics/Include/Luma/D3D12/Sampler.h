#pragma once
#include "DescriptorHeap.h"
#include "Rendering/Sampler.h"

namespace Luma::D3D12
{
    class GpuDevice;
    class Sampler : public Luma::Sampler
    {
    public:
        bool Initialize(const SamplerCreateInfo& createInfo) override;
        void Destroy() override;
    private:
        DescriptorHandle m_Handle = nullptr;
        GpuDevice* m_Device = nullptr;
    };
}
