#include "Luma/Components/StaticMeshRenderer.h"
#include "Luma/Asset/StaticMesh.h"
#include "Luma/Rendering/BindingSet.h"
#include "Luma/Rendering/Camera.h"
#include "Luma/Rendering/CommandBuffer.h"

namespace Luma
{
    struct alignas(16) FCameraData
    {
        FMatrix4f viewMatrix;
        FMatrix4f inverseViewMatrix;
        FMatrix4f projectionMatrix;
        FMatrix4f inverseProjectionMatrix;
        FMatrix4f viewProjectionMatrix;
        FMatrix4f inverseViewProjectionMatrix;
        FVector4f cameraPos;
        FVector4f cameraDir;
    };
    
    struct alignas(16) FObjectData
    {
        FMatrix4f localToWorldMatrix;
        FMatrix4f normalMatrix;
    };
    
    struct alignas(16) FDirectionalLightData
    {
        FVector3f color;
        float intensity;
        FVector3f direction;
        float padding;
    };
    
    struct alignas(16) FAmbientLightData
    {
        FVector3f color;
        float intensity;
    };
    
    struct alignas(16) FSceneData
    {
        FDirectionalLightData dirLight;
        FAmbientLightData ambientLight;
        
    };
    
    void FStaticMeshRenderer::onInit()
    {

    }

    void FStaticMeshRenderer::onDestroy()
    {
        m_SceneUniformBuffer->destroy();
        m_ObjectUniformBuffer->destroy();
        m_CameraUniformBuffer->destroy();
        m_BindingSet1->destroy();
        m_BindingSet2->destroy();
    }

    void FStaticMeshRenderer::onPreRender(ICommandBuffer* cmdBuffer)
    {
    
    }

    void FStaticMeshRenderer::onRender(ICommandBuffer* cmdBuffer, const FCamera& camera)
    {
        if (!m_StaticMesh) return;

        const auto vertexBuffer = m_StaticMesh->getVertexBuffer();
        const auto indexBuffer = m_StaticMesh->getIndexBuffer();
        const auto materialSlots = m_StaticMesh->getMaterialSlots();
        if (!vertexBuffer || !indexBuffer) return;
        if (materialSlots.count() <= 0) return;

        const auto width = camera.getWidth();
        const auto height = camera.getHeight();

        cmdBuffer->setViewport(FViewport(0, 0, width, height, 0, 1));
        cmdBuffer->setScissor(FScissor(0, 0, width, height));

        const auto& perMaterialMeshParts = m_StaticMesh->getPerMaterialMeshParts();

        for (const auto& [index, slot] : materialSlots)
        {
            Ref<FMaterial> material = slot.material;
            if (!material) return;

            cmdBuffer->bindBindingSet(m_BindingSet1, material->getShader());
            cmdBuffer->bindBindingSet(m_BindingSet2, material->getShader());
            cmdBuffer->bindMaterial(material);

            const auto& meshParts = perMaterialMeshParts[index];

            for (const FMeshPart& meshPart : meshParts)
            {
                cmdBuffer->bindVertexBuffer(vertexBuffer, meshPart.vertexOffset);
                cmdBuffer->bindIndexBuffer(indexBuffer, meshPart.indexOffset, EIndexFormat::Uint32);
                cmdBuffer->drawIndexed(FDrawIndexedCommand(meshPart.indexSize / sizeof(uint32_t), 1, 0, 0, 0));
            }
        }
    }

    Ref<FStaticMesh> FStaticMeshRenderer::getMesh() const
    {
         return m_StaticMesh;
    }

    void FStaticMeshRenderer::setMesh(Ref<FStaticMesh> newMesh)
    {
        m_StaticMesh = newMesh;
    }

}
