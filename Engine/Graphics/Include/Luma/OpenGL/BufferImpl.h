#pragma once
#include "Luma/Rendering/Buffer.h"

namespace Luma::OpenGL
{
    class FGpuDeviceImpl;

    class FBufferImpl final : public IBuffer
    {
    public:
        bool initialize(const FBufferDesc& bufferDesc) override;
        void destroy() override;
        void* map() override;
        void unmap(const void* ptr) override;
        uint64_t getDeviceAddress() const override;
    private:
        FGpuDeviceImpl* m_Device = nullptr;
        uint32_t m_Handle = 0xFFFFFFFF;
    };
}
