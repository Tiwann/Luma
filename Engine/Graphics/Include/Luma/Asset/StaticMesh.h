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
        String name;
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
        bool loadFromFile(StringView filepath, RHI::Device* device);

        void setMaterial(uint32_t slot, Ref<Material> material);

        Ref<Material> getMaterial(uint32_t slot);
        const HashMap<uint32_t, FMaterialSlot>& getMaterialSlots() const { return m_MaterialSlots; }
        const HashMap<uint32_t, Array<FMeshPart>>& getPerMaterialMeshParts() const { return m_PerMaterialData; }

        WeakRef<RHI::Buffer> getVertexBuffer() const;
        WeakRef<RHI::Buffer> getIndexBuffer() const;
    private:
        Ref<RHI::Buffer> m_VertexBuffer = nullptr;
        Ref<RHI::Buffer> m_IndexBuffer = nullptr;
        HashMap<uint32_t, Array<FMeshPart>> m_PerMaterialData;
        HashMap<uint32_t, FMaterialSlot> m_MaterialSlots;
        HashMap<uint32_t, FMaterialTextures> m_Textures;
    };
}
