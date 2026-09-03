#pragma once
#include "MaterialType.h"
#include "Luma/Containers/HashMap.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Rendering/BindingType.h"
#include "Luma/Rendering/GpuDevice.h"
#include "Luma/Rendering/BindingSet.h"
#include "Luma/Rendering/ShaderProgram.h"
#include "Luma/Runtime/Asset.h"

namespace Luma
{
    struct FMaterialDesc
    {
        IGpuDevice* device = nullptr;
        IShaderProgram* shader = nullptr;
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
        const IShaderProgram* getShader() const { return m_Shader; }
        const IBindingSet* getBindingSet() const { return m_BindingSet; }
        void setMaterialType(const EMaterialType materialType) { m_MaterialType = materialType; }
        EMaterialType getMaterialType() const { return m_MaterialType; }

    private:
        EMaterialType m_MaterialType = EMaterialType::Opaque;
        Ref<IGpuDevice> m_Device = nullptr;
        Ref<IShaderProgram> m_Shader = nullptr;
        Ref<IBindingSet> m_BindingSet = nullptr;
        //THashMap<EMaterialType, Ref<IGraphicsPipeline>> m_Pipelines;
    };
}
