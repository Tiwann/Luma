#pragma once
#include "MaterialType.h"
#include "Luma/Containers/HashMap.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Rendering/BindingType.h"
#include "Luma/Rendering/Device.h"
#include "Luma/Rendering/Shader.h"
#include "Luma/Runtime/Asset.h"

namespace Luma
{
    struct MaterialDesc
    {
        RHI::Device* device = nullptr;
        RHI::Shader* shader = nullptr;
    };

    class Material : public Asset
    {
    public:
        bool initialize(const MaterialDesc& materialDesc);
        void destroy() override;

        void setSampler(const FString& name, const RHI::Sampler* sampler);
        void setTexture(const FString& name, const RHI::Texture* texture, BindingType bindingType);
        void setCombinedTextureSampler(const FString& name, const RHI::Sampler* sampler, const RHI::Texture* texture);
        void setBuffer(const FString& name, const RHI::Buffer* buffer, uint64_t offset, uint64_t size);

        AssetType getAssetType() const override { return AssetType::Material; }
        Ref<RHI::Shader> getShader() const { return m_Shader; }
        //const IBindingGroup* getBindingSet() const { return m_BindingSet; }
        void setMaterialType(const EMaterialType materialType) { m_MaterialType = materialType; }
        EMaterialType getMaterialType() const { return m_MaterialType; }

    private:
        EMaterialType m_MaterialType = EMaterialType::Opaque;
        Ref<RHI::Device> m_Device = nullptr;
        Ref<RHI::Shader> m_Shader = nullptr;
        //Ref<IBindingGroup> m_BindingSet = nullptr;
        //THashMap<EMaterialType, Ref<IGraphicsPipeline>> m_Pipelines;
    };
}
