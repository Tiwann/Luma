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
        Ref<FMaterial> material;
    };

    class FStaticMesh final : public IAsset, public IRefCounted<FStaticMesh>
    {
    public:
        FStaticMesh() = default;
        ~FStaticMesh() override = default;

        EAssetType getAssetType() const override { return EAssetType::StaticMesh; }
        bool loadFromFile(FStringView filepath, IRenderDevice* device);

        void setMaterial(uint32_t slot, Ref<FMaterial> material);

        Ref<FMaterial> getMaterial(uint32_t slot);
        const THashMap<uint32_t, FMaterialSlot>& getMaterialSlots() const { return m_MaterialSlots; }
        const THashMap<uint32_t, TArray<FMeshPart>>& getPerMaterialMeshParts() const { return m_PerMaterialData; }

        WeakRef<IBuffer> getVertexBuffer() const;
        WeakRef<IBuffer> getIndexBuffer() const;
    private:
        Ref<IBuffer> m_VertexBuffer = nullptr;
        Ref<IBuffer> m_IndexBuffer = nullptr;
        THashMap<uint32_t, TArray<FMeshPart>> m_PerMaterialData;
        THashMap<uint32_t, FMaterialSlot> m_MaterialSlots;
    };
}
