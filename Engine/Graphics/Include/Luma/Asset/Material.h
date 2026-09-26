#pragma once
#include "MaterialType.h"
#include "Luma/Containers/HashMap.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Rendering/BindingType.h"
#include "Luma/Rendering/Forward.h"
#include "Luma/Runtime/Asset.h"

namespace Luma
{
    class Material : public Asset
    {
    public:
        Material() = default;
        Material(Ref<Shader> shader);
        ~Material();

        AssetType getAssetType() const override { return AssetType::Material; }
        Ref<Shader> getShader() const { return m_Shader; }

        void setMaterialType(const MaterialType materialType) { m_MaterialType = materialType; }
        MaterialType getMaterialType() const { return m_MaterialType; }

    private:
        MaterialType m_MaterialType = MaterialType::Opaque;
        Ref<Shader> m_Shader = nullptr;
        Ref<BindingGroup> m_BindingGroup = nullptr;
    };
}
