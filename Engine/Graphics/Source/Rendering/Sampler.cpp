#include "Luma/Rendering/Sampler.h"


namespace Luma::RHI
{
    SamplerDesc& SamplerDesc::withDevice(Device* device)
    {
        this->device = device;
        return *this;
    }

    SamplerDesc& SamplerDesc::withAddressModeUVW(const SamplerAddressMode u, const SamplerAddressMode v,
                                                 const SamplerAddressMode w)
    {
        addressModeU = u;
        addressModeV = v;
        addressModeW = w;
        return *this;
    }

    SamplerDesc& SamplerDesc::withAddressMode(const SamplerAddressMode in)
    {
        addressModeU = in;
        addressModeV = in;
        addressModeW = in;
        return *this;
    }

    SamplerDesc& SamplerDesc::withFilter(const Filter inMinFilter, const Filter inMagFilter)
    {
        minFilter = inMinFilter;
        magFilter = inMagFilter;
        return *this;
    }

    SamplerDesc& SamplerDesc::withLODRange(const float min, const float max)
    {
        minLod = min;
        maxLod = max;
        return *this;
    }
}
