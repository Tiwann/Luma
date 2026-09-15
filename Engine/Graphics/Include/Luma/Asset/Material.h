#pragma once
#include "MaterialType.h"
#include "Luma/Containers/HashMap.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Rendering/BindingType.h"
#include "Luma/Rendering/GPUDevice.h"
#include "Luma/Rendering/Shader.h"
#include "Luma/Runtime/Asset.h"

namespace Luma
{
    struct FMaterialDesc
    {
        IGPUDevice* device = nullptr;
        IShader* shader = nullptr;
    };

    class FMaterial : public IAsset
    {
    public:
        bool initialize(const FMaterialDesc& materialDesc);
        void destroy() override;

        void setSampler(const FString& name, const ISampler* sampler);
        void setTexture(const FString& name, const ITexture* texture, EBindingType bindingType);
        void setCombinedTextureSampler(const FString& name, const ISampler* sampler, const ITexture* texture);
        void setBuffer(const FString& name, const IBuffer* buffer, uint64_t offset, uint64_t size);

        EAssetType getAssetType() const override { return EAssetType::Material; }
        const IShader* getShader() const { return m_Shader; }
        //const IBindingGroup* getBindingSet() const { return m_BindingSet; }
        void setMaterialType(const EMaterialType materialType) { m_MaterialType = materialType; }
        EMaterialType getMaterialType() const { return m_MaterialType; }

    private:
        EMaterialType m_MaterialType = EMaterialType::Opaque;
        Ref<IGPUDevice> m_Device = nullptr;
        Ref<IShader> m_Shader = nullptr;
        //Ref<IBindingGroup> m_BindingSet = nullptr;
        //THashMap<EMaterialType, Ref<IGraphicsPipeline>> m_Pipelines;
    };
}
