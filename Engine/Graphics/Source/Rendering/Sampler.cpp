#include "Luma/Rendering/Sampler.h"
#include <unordered_map>

static void hashCombine(size_t& seed, const size_t value)
{
    seed ^= value + 0x9e3779b97f4a7c15ull + (seed << 6) + (seed >> 2);
}

template<>
struct std::hash<Luma::FSamplerDesc>
{
    size_t operator()(const Luma::FSamplerDesc& desc) const noexcept
    {
        std::size_t seed = 0;

        hashCombine(seed, std::hash<void*>{}(desc.device));

        hashCombine(seed, std::hash<uint32_t>{}(static_cast<uint32_t>(desc.addressModeU)));
        hashCombine(seed, std::hash<uint32_t>{}(static_cast<uint32_t>(desc.addressModeV)));
        hashCombine(seed, std::hash<uint32_t>{}(static_cast<uint32_t>(desc.addressModeW)));

        hashCombine(seed, std::hash<uint32_t>{}(static_cast<uint32_t>(desc.minFilter)));
        hashCombine(seed, std::hash<uint32_t>{}(static_cast<uint32_t>(desc.magFilter)));
        hashCombine(seed, std::hash<uint32_t>{}(static_cast<uint32_t>(desc.mipmapFilter)));

        hashCombine(seed, std::hash<uint32_t>{}(static_cast<uint32_t>(desc.compareOp)));

        hashCombine(seed, std::hash<bool>{}(desc.anisotropyEnable));
        hashCombine(seed, std::hash<bool>{}(desc.compareEnable));
        hashCombine(seed, std::hash<bool>{}(desc.unnormalizedCoordinates));

        hashCombine(seed, std::hash<float>{}(desc.minLod));
        hashCombine(seed, std::hash<float>{}(desc.maxLod));

        return seed;
    }
};

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
