#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/Buffer.h"
#include "DekoFwd.h"

namespace Luma::Deko3d
{
    class FGpuDeviceImpl;

    class LUMA_GRAPHICS_API FBufferImpl final : public IBuffer
    {
    public:
        FBufferImpl() = default;
        ~FBufferImpl() override = default;

        bool initialize(const FBufferDesc& bufferDesc) override;
        void destroy() override;

        void* map() override;
        void unmap(const void* ptr) override;
        uint64_t getDeviceAddress() const override;

        DkMemBlock getHandle() const { return m_Handle; }
    private:
        FGpuDeviceImpl* m_Device = nullptr;
        DkMemBlock m_Handle = nullptr;
    };
}
