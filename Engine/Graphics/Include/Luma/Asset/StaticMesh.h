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
    struct MeshPart
    {
        uint64_t vertexOffset = 0;
        uint64_t vertexSize = 0;
        uint64_t indexOffset = 0;
        uint64_t indexSize = 0;
    };

    struct MaterialSlot
    {
        String name;
        Ref<Material> material;
    };

    struct MaterialTextures
    {
        Ref<Texture> baseColor;
        Ref<Texture> metallicRoughnessAO;
        Ref<Texture> normal;
        Ref<Texture> emission;
    };

    class StaticMesh final : public Asset
    {
    public:
        StaticMesh() = default;
        ~StaticMesh() override = default;

        void destroy() override;

        AssetType getAssetType() const override { return AssetType::StaticMesh; }
        bool loadFromFile(StringView filepath, Device* device);

        void setMaterial(uint32_t slot, Ref<Material> material);

        Ref<Material> getMaterial(uint32_t slot);
        const HashMap<uint32_t, MaterialSlot>& getMaterialSlots() const { return m_MaterialSlots; }
        const HashMap<uint32_t, Array<MeshPart>>& getPerMaterialMeshParts() const { return m_PerMaterialData; }

        WeakRef<Buffer> getVertexBuffer() const;
        WeakRef<Buffer> getIndexBuffer() const;
    private:
        Ref<Buffer> m_VertexBuffer = nullptr;
        Ref<Buffer> m_IndexBuffer = nullptr;
        HashMap<uint32_t, Array<MeshPart>> m_PerMaterialData;
        HashMap<uint32_t, MaterialSlot> m_MaterialSlots;
        HashMap<uint32_t, MaterialTextures> m_Textures;
    };
}
