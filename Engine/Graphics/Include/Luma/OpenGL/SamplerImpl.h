#pragma once
#include "Common.h"
#include "Rendering/Sampler.h"

namespace Luma::OpenGL
{
    class RenderDevice;

    class SamplerImpl final : public Luma::Sampler
    {
    public:
        bool Initialize(const SamplerCreateInfo& createInfo) override;
        void Destroy() override;

        uint32_t GetHandle() const { return m_Handle; }
        void Bind(uint32_t unit) const;
    private:
        RenderDevice* m_Device = nullptr;
        uint32_t m_Handle = INVALID_HANDLE<uint32_t>;
    };
}
