#include "Luma/Rendering/Sampler.h"


namespace Luma
{
    FSamplerDesc& FSamplerDesc::withDevice(IRenderDevice* device)
    {
        this->device = device;
        return *this;
    }

    FSamplerDesc& FSamplerDesc::withAddressModeUVW(const ESamplerAddressMode u, const ESamplerAddressMode v,
                                                 const ESamplerAddressMode w)
    {
        addressModeU = u;
        addressModeV = v;
        addressModeW = w;
        return *this;
    }

    FSamplerDesc& FSamplerDesc::withAddressMode(const ESamplerAddressMode in)
    {
        addressModeU = in;
        addressModeV = in;
        addressModeW = in;
        return *this;
    }

    FSamplerDesc& FSamplerDesc::withFilter(const EFilter inMinFilter, const EFilter inMagFilter)
    {
        minFilter = inMinFilter;
        magFilter = inMagFilter;
        return *this;
    }

    FSamplerDesc& FSamplerDesc::withLODRange(const float min, const float max)
    {
        minLod = min;
        maxLod = max;
        return *this;
    }
}
