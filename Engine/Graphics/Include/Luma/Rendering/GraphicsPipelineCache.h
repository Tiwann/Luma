#pragma once
#include "InputLayout.h"
#include "Luma/Asset/MaterialType.h"
#include "Luma/Containers/HashMap.h"
#include "Luma/Memory/Ref.h"

namespace Luma
{
    struct IShader;
    struct IGraphicsPipeline;
    struct IRenderDevice;

    struct FGraphicsPipelineKey
    {
        IShader* shader;
        EMaterialType materialType;
        FVertexInputLayout vertexInputLayout;
        EFormat depthFormat;
        EFormat colorFormats[8];
        uint32_t colorFormatCount;
        bool operator==(const FGraphicsPipelineKey&) const = default;
    };

    template<>
    struct THasher<FGraphicsPipelineKey>
    {
        static void hashCombine(uint64_t& seed, const uint64_t value)
        {
            seed ^= value + 0x9e3779b97f4a7c15ull + (seed << 6) + (seed >> 2);
        }

        uint64_t operator()(const FGraphicsPipelineKey& key) const
        {
            uint64_t seed = 0;
            hashCombine(seed, reinterpret_cast<uint64_t>(key.shader));
            hashCombine(seed, static_cast<uint64_t>(key.materialType));
            hashCombine(seed, THasher<FVertexInputLayout>{}(key.vertexInputLayout));
            hashCombine(seed, static_cast<uint64_t>(key.depthFormat));
            for (uint32_t i = 0; i < key.colorFormatCount; ++i)
                hashCombine(seed, static_cast<uint64_t>(key.colorFormats[i]));
            return seed;
        }
    };

    class FGraphicsPipelineCache
    {
    public:
        FGraphicsPipelineCache(Ref<IRenderDevice> renderDevice);

        void destroy();
        Ref<IGraphicsPipeline> getOrCreate(const FGraphicsPipelineKey& key);
    private:
        Ref<IRenderDevice> m_Device = nullptr;
        THashMap<FGraphicsPipelineKey, Ref<IGraphicsPipeline>> m_Cache;
    };
}
