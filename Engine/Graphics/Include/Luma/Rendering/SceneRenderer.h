#pragma once
#include "Buffer.h"
#include "Texture.h"
#include "Luma/Asset/Material.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Memory/RefCounted.h"

namespace Luma
{
    struct ICommandBuffer;
    class FScene;

    class FGBuffer : public IRefCounted<FGBuffer>
    {
    public:
        bool initialize(IRenderDevice* device, uint32_t width, uint32_t height);
        void destroy();

        Ref<ITexture> getAlbedoTexture() const { return m_Albedo; }
        Ref<ITexture> getMetallicRoughnessTexture() const { return m_MetallicRoughness; }
        Ref<ITexture> getNormalTexture() const { return m_Normal; }
        Ref<ITexture> getEmissionTexture() const { return m_Emission; }
        Ref<ITexture> getPositionTexture() const { return m_Position; }
        Ref<ITexture> getDepthStencilTexture() const { return m_DepthStencil; }
    private:
        Ref<ITexture> m_Albedo = nullptr;
        Ref<ITexture> m_MetallicRoughness = nullptr;
        Ref<ITexture> m_Normal = nullptr;
        Ref<ITexture> m_Emission = nullptr;
        Ref<ITexture> m_Position = nullptr;
        Ref<ITexture> m_DepthStencil = nullptr;
    };

    struct FSceneRendererDesc
    {
        IRenderDevice* device = nullptr;
        uint32_t width = 0;
        uint32_t height = 0;
    };

    class FSceneRenderer : public IRefCounted<FSceneRenderer>
    {
    public:
        bool initialize(const FSceneRendererDesc& desc);
        void destroy();

        void resize(uint32_t width, uint32_t height);
        void begin();
        void end();
        void render(ICommandBuffer* cmdBuffer);

        FScene* getScene() const { return m_Scene; }
        void setScene(FScene* scene) { m_Scene = scene; }

        Ref<FGBuffer> getGBuffer() const { return m_GBuffer; }
        Ref<ITexture> getFinalTexture() const { return m_FinalTexture; }
        Ref<IShader> getShader() const { return m_GeometryPassShader; }
    private:
        bool m_Begin = false;
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
        IRenderDevice* m_Device = nullptr;
        FScene* m_Scene = nullptr;
        Ref<FGBuffer> m_GBuffer = nullptr;
        Ref<ITexture> m_FinalTexture = nullptr;
        Ref<IBuffer> m_CameraBuffer = nullptr;
        Ref<IBuffer> m_ObjectBuffer = nullptr;
        Ref<IBuffer> m_SceneBuffer = nullptr;
        Ref<IBindingSet> m_BindingSet1 = nullptr;
        Ref<IBindingSet> m_BindingSet2 = nullptr;
        Ref<IGraphicsPipeline> m_GeometryPipeline = nullptr;
        Ref<IShader> m_GeometryPassShader = nullptr;
    };
}
