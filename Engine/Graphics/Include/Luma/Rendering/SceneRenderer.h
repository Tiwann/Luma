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
        bool initialize(IGPUDevice* device, uint32_t width, uint32_t height);
        void destroy();

        Ref<Texture> getAlbedoTexture() const { return m_Albedo; }
        Ref<Texture> getMetallicRoughnessTexture() const { return m_MetallicRoughness; }
        Ref<Texture> getNormalTexture() const { return m_Normal; }
        Ref<Texture> getEmissionTexture() const { return m_Emission; }
        Ref<Texture> getPositionTexture() const { return m_Position; }
        Ref<Texture> getDepthStencilTexture() const { return m_DepthStencil; }
    private:
        Ref<Texture> m_Albedo = nullptr;
        Ref<Texture> m_MetallicRoughness = nullptr;
        Ref<Texture> m_Normal = nullptr;
        Ref<Texture> m_Emission = nullptr;
        Ref<Texture> m_Position = nullptr;
        Ref<Texture> m_DepthStencil = nullptr;
    };

    struct FSceneRendererDesc
    {
        IGPUDevice* device = nullptr;
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
        Ref<Texture> getFinalTexture() const { return m_FinalTexture; }
        Ref<IShader> getShader() const { return m_GeometryPassShader; }
    private:
        bool m_Begin = false;
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
        IGPUDevice* m_Device = nullptr;
        FScene* m_Scene = nullptr;
        Ref<FGBuffer> m_GBuffer = nullptr;
        Ref<Texture> m_FinalTexture = nullptr;
        Ref<IBuffer> m_CameraBuffer = nullptr;
        Ref<IBuffer> m_ObjectBuffer = nullptr;
        Ref<IBuffer> m_SceneBuffer = nullptr;
        Ref<BindingGroup> m_BindingSet1 = nullptr;
        Ref<BindingGroup> m_BindingSet2 = nullptr;
        Ref<IRenderPipeline> m_GeometryPipeline = nullptr;
        Ref<IShader> m_GeometryPassShader = nullptr;
    };
}
