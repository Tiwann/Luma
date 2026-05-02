#pragma once
#include "Luma/Memory/Ref.h"
#include "Luma/Runtime/Component.h"
#include "Luma/Rendering/Buffer.h"
#include "Luma/Rendering/BindingSet.h"
#include "Luma/Rendering/Sampler.h"
#include "Luma/Rendering/Shader.h"
#include "Luma/Asset/StaticMesh.h"
#include "Luma/Rendering/GraphicsPipeline.h"

namespace Luma
{
    class FStaticMesh;
    struct IShader;
    struct ISampler;
    struct IBindingSet;
    struct IBuffer;
    struct IGraphicsPipeline;

    class FStaticMeshRenderer : public IComponent
    {
    public:
        FStaticMeshRenderer() = default;

        void onInit() override;
        void onDestroy() override;
        void onPreRender(ICommandBuffer* cmdBuffer) override;
        void onRender(ICommandBuffer* cmdBuffer, const FCamera& camera) override;

        Ref<FStaticMesh> getMesh() const;
        void setMesh(Ref<FStaticMesh> newMesh);
    private:
        Ref<FStaticMesh> m_StaticMesh = nullptr;
        Ref<IShader> m_Shader = nullptr;
        Ref<ISampler> m_Sampler = nullptr;
        Ref<IGraphicsPipeline> m_Pipeline = nullptr;
        Ref<IBuffer> m_SceneUniformBuffer = nullptr;
        Ref<IBuffer> m_ObjectUniformBuffer = nullptr;
        Ref<IBuffer> m_CameraUniformBuffer = nullptr;
        Ref<IBindingSet> m_BindingSet1 = nullptr;
        Ref<IBindingSet> m_BindingSet2 = nullptr;
    };
}
