#pragma once
#include "Material.h"
#include "Luma/Containers/String.h"
#include "Luma/Containers/StringView.h"
#include "Luma/Containers/HashMap.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Rendering/Buffer.h"
#include "Luma/Runtime/Asset.h"
#include <cstdint>


namespace Luma
{
    struct FMeshPart
    {
        uint64_t vertexOffset = 0;
        uint64_t vertexSize = 0;
        uint64_t indexOffset = 0;
        uint64_t indexSize = 0;
    };

    struct FMaterialSlot
    {
        FString name;
        Ref<Material> material;
    };

    struct FMaterialTextures
    {
        Ref<RHI::Texture> baseColor;
        Ref<RHI::Texture> metallicRoughnessAO;
        Ref<RHI::Texture> normal;
        Ref<RHI::Texture> emission;
    };

    class FStaticMesh final : public Asset
    {
    public:
        FStaticMesh() = default;
        ~FStaticMesh() override = default;

        void destroy() override;

        AssetType getAssetType() const override { return AssetType::StaticMesh; }
        bool loadFromFile(FStringView filepath, RHI::Device* device);

        void setMaterial(uint32_t slot, Ref<Material> material);

        Ref<Material> getMaterial(uint32_t slot);
        const THashMap<uint32_t, FMaterialSlot>& getMaterialSlots() const { return m_MaterialSlots; }
        const THashMap<uint32_t, TArray<FMeshPart>>& getPerMaterialMeshParts() const { return m_PerMaterialData; }

        WeakRef<RHI::Buffer> getVertexBuffer() const;
        WeakRef<RHI::Buffer> getIndexBuffer() const;
    private:
        Ref<RHI::Buffer> m_VertexBuffer = nullptr;
        Ref<RHI::Buffer> m_IndexBuffer = nullptr;
        THashMap<uint32_t, TArray<FMeshPart>> m_PerMaterialData;
        THashMap<uint32_t, FMaterialSlot> m_MaterialSlots;
        THashMap<uint32_t, FMaterialTextures> m_Textures;
    };
}
