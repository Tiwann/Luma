#include "Luma/Rendering/SceneRenderer.h"

#include "Luma/Components/CameraComponent.h"
#include "Luma/Components/StaticMeshRenderer.h"
#include "Luma/Components/TransformComponent.h"
#include "Luma/Rendering/CommandBuffer.h"
#include "Luma/Rendering/RenderPipeline.h"
#include "Luma/Rendering/InputLayout.h"
#include "Luma/Rendering/RenderDevice.h"
#include "Luma/Rendering/RenderPassDesc.h"
#include "Luma/Runtime/Entity.h"
#include "Luma/Runtime/Path.h"
#include "Luma/Runtime/Scene.h"
#include "Luma/Runtime/SceneTemplates.hxx"

namespace Luma
{
    struct alignas(16) FCameraData
    {
        FMatrix4<float> viewMatrix;
        FMatrix4<float> inverseViewMatrix;
        FMatrix4<float> projectionMatrix;
        FMatrix4<float> inverseProjectionMatrix;
        FMatrix4<float> viewProjectionMatrix;
        FMatrix4<float> inverseViewProjectionMatrix;
        FVector4<float> cameraPos;
        FVector4<float> cameraDir;
    };

    struct alignas(16) FObjectData
    {
        FMatrix4<float> worldSpaceMatrix;
        FMatrix4<float> normalMatrix;
    };

    struct alignas(16) FDirectionalLightData
    {
        FVector3<float> color;
        float intensity;
        FVector3<float> direction;
    };

    struct alignas(16) FAmbientLightData
    {
        FVector3<float> color;
        float intensity;
    };

    struct alignas(16) FSceneData
    {
        FDirectionalLightData directionalLight;
        FAmbientLightData ambientLight;
    };


    bool FGBuffer::initialize(IRenderDevice* device, uint32_t width, uint32_t height)
    {
        if (!device) return false;
        if (width == 0 || height == 0) return false;

        FTextureDesc textureDesc = FTextureDesc::texture2D(width, height, EFormat::None, 1, 8);

        textureDesc.format = EFormat::R8G8B8A8_UNORM;
        textureDesc.usageFlags = ETextureUsageBits::Sampled | ETextureUsageBits::ColorTarget;
        m_Albedo = device->createTexture(textureDesc);
        if (!m_Albedo) return false;

        textureDesc.format = EFormat::R8G8B8A8_UNORM;
        textureDesc.usageFlags = ETextureUsageBits::Sampled | ETextureUsageBits::ColorTarget;
        m_MetallicRoughness = device->createTexture(textureDesc);
        if (!m_MetallicRoughness) return false;

        textureDesc.format = EFormat::R32G32B32A32_FLOAT;
        textureDesc.usageFlags = ETextureUsageBits::Sampled | ETextureUsageBits::ColorTarget;
        m_Normal = device->createTexture(textureDesc);
        if (!m_Normal) return false;

        textureDesc.format = EFormat::R8G8B8A8_UNORM;
        textureDesc.usageFlags = ETextureUsageBits::Sampled | ETextureUsageBits::ColorTarget;
        m_Emission = device->createTexture(textureDesc);
        if (!m_Emission) return false;

        textureDesc.format = EFormat::R8G8B8A8_UNORM;
        textureDesc.usageFlags = ETextureUsageBits::Sampled | ETextureUsageBits::ColorTarget;
        m_Position = device->createTexture(textureDesc);
        if (!m_Position) return false;

        textureDesc.format = EFormat::D32_FLOAT_S8_UINT;
        textureDesc.usageFlags = ETextureUsageBits::DepthStencilTarget;
        m_DepthStencil = device->createTexture(textureDesc);
        if (!m_DepthStencil) return false;
        return true;
    }

    void FGBuffer::destroy()
    {
        m_Albedo = nullptr;
        m_MetallicRoughness = nullptr;
        m_Normal = nullptr;
        m_Emission = nullptr;
        m_Position = nullptr;
        m_DepthStencil = nullptr;
    }

    bool FSceneRenderer::initialize(const FSceneRendererDesc& desc)
    {
        if (!desc.device) return false;
        if (desc.height == 0) return false;
        if (desc.width == 0) return false;

        IRenderDevice* device = desc.device;

        m_GBuffer = new FGBuffer();
        if (!m_GBuffer->initialize(device, desc.width, desc.height))
        {
            m_GBuffer = nullptr;
            return false;
        }

        const FTextureDesc textureDesc = FTextureDesc::texture2D(desc.width, desc.height, EFormat::R8G8B8A8_SRGB);
        m_FinalTexture = device->createTexture(textureDesc);
        if (!m_FinalTexture)
        {
            m_FinalTexture = nullptr;
            m_GBuffer = nullptr;
            return false;
        }

        FBufferDesc uniformBufferDesc;
        uniformBufferDesc.alwaysMapped = true;
        uniformBufferDesc.usage = EBufferUsage::UniformBuffer;

        FBufferDesc cameraBufferDesc = uniformBufferDesc;
        cameraBufferDesc.size = sizeof(FCameraData);
        m_CameraBuffer = device->createBuffer(cameraBufferDesc);

        FBufferDesc objectBufferDesc = uniformBufferDesc;
        objectBufferDesc.size = sizeof(FObjectData);
        m_ObjectBuffer = device->createBuffer(objectBufferDesc);

        FBufferDesc sceneBufferDesc = uniformBufferDesc;
        sceneBufferDesc.size = sizeof(FSceneData);
        m_SceneBuffer = device->createBuffer(sceneBufferDesc);

        FShaderDesc shaderDesc;
        shaderDesc.device = device;
        shaderDesc.moduleName = "GeometryPass";
        shaderDesc.filepath = FPath::getEngineShaderPath("Deferred/GeometryPass.slang");
        shaderDesc.stageFlags = EShaderStageBits::Vertex | EShaderStageBits::Fragment;
        shaderDesc.defines = {{"DECLARE_CAMERA_BUFFER", "1"}, {"DECLARE_SCENE_BUFFER", "1"}, {"DECLARE_OBJECT_BUFFER", "1"}};
        m_GeometryPassShader = device->createShader(shaderDesc);

        //FShaderDesc shaderDesc;
        //shaderDesc.device = device;
        //shaderDesc.moduleName = "LightingPass";
        //shaderDesc.filepath = FPath::getEngineShaderPath("Deferred/LightingPass.slang");
        //shaderDesc.stageFlags = EShaderStageBits::Vertex | EShaderStageBits::Fragment;
        //m_GeometryPassShader = device->createShader(shaderDesc);

        FVertexInputLayout vertexLayout;
        vertexLayout.addInputBinding(0, EVertexInputRate::Vertex);
        vertexLayout.addInputAttribute("POSITION", EShaderDataType::Float3, 0);
        vertexLayout.addInputAttribute("TEXCOORDINATE", EShaderDataType::Float2, 0);
        vertexLayout.addInputAttribute("NORMAL", EShaderDataType::Float3, 0);
        vertexLayout.addInputAttribute("TANGENT", EShaderDataType::Float3, 0);
        vertexLayout.addInputAttribute("COLOR", EShaderDataType::Float4, 0);

        FGraphicsPipelineDesc pipelineDesc;
        pipelineDesc.device = device;
        pipelineDesc.shaderProgram = m_GeometryPassShader;
        pipelineDesc.depthStencil.depthWriteEnable = true;
        pipelineDesc.depthStencil.depthTestEnable = true;
        pipelineDesc.inputLayout = vertexLayout;
        pipelineDesc.colorFormats[0] = EFormat::R8G8B8A8_UNORM;
        pipelineDesc.colorFormats[1] = EFormat::R8G8B8A8_UNORM;
        pipelineDesc.colorFormats[2] = EFormat::R32G32B32A32_FLOAT;
        pipelineDesc.colorFormats[3] = EFormat::R8G8B8A8_UNORM;
        pipelineDesc.colorFormats[4] = EFormat::R8G8B8A8_UNORM;
        pipelineDesc.colorFormatCount = 5;
        pipelineDesc.depthFormat = EFormat::D32_FLOAT_S8_UINT;
        pipelineDesc.multisample = {8};
        m_GeometryPipeline = device->createGraphicsPipeline(pipelineDesc);

        m_BindingSet1 = m_GeometryPassShader->createBindingSet(1);
        m_BindingSet2 = m_GeometryPassShader->createBindingSet(2);

        m_BindingSet1->bindBuffer("u_ObjectData", m_ObjectBuffer, 0, sizeof(FObjectData));
        m_BindingSet2->bindBuffer("u_CameraData", m_CameraBuffer, 0, sizeof(FCameraData));
        m_BindingSet2->bindBuffer("u_SceneData", m_SceneBuffer, 0, sizeof(FSceneData));

        m_Device = desc.device;
        m_Width = desc.width;
        m_Height = desc.height;
        return true;
    }

    void FSceneRenderer::destroy()
    {
        m_GBuffer = nullptr;
        m_FinalTexture = nullptr;
        m_CameraBuffer = nullptr;
        m_ObjectBuffer = nullptr;
        m_SceneBuffer = nullptr;
        m_BindingSet1 = nullptr;
        m_BindingSet2 = nullptr;
        m_GeometryPipeline = nullptr;
        m_GeometryPassShader = nullptr;
    }

    void FSceneRenderer::resize(const uint32_t width, const uint32_t height)
    {
        m_Device->waitIdle();

        FSceneRendererDesc rendererDesc;
        rendererDesc.device = m_Device;
        rendererDesc.width = width;
        rendererDesc.height = height;
        initialize(rendererDesc);
    }

    void FSceneRenderer::begin()
    {
        LUMA_ASSERT(!m_Begin, "begin/end mismatch!");
        m_Begin = true;
    }

    void FSceneRenderer::end()
    {
        LUMA_ASSERT(m_Begin, "begin/end mismatch!");
        if (!m_Scene) return;

        FCameraComponent* cameraComponent = m_Scene->getFirstComponent<FCameraComponent>();
        if (!cameraComponent) return;

        FTransformComponent* transformComponent = cameraComponent->getTransformComponent();
        if (!transformComponent) return;

        const FMatrix4<float>& viewMatrix = cameraComponent->getViewMatrix();
        const FMatrix4<float>& projectionMatrix = cameraComponent->getProjectionMatrix();

        const FVector3<float>& cameraPosition = transformComponent->getPosition();
        const FVector3<float>& cameraDirection = transformComponent->getForward();

        FCameraData cameraData;
        cameraData.viewMatrix = viewMatrix;
        cameraData.inverseViewMatrix = viewMatrix.inverted();
        cameraData.projectionMatrix = projectionMatrix;
        cameraData.inverseProjectionMatrix = projectionMatrix.inverted();
        cameraData.viewProjectionMatrix = projectionMatrix * viewMatrix;
        cameraData.inverseViewProjectionMatrix = cameraData.viewProjectionMatrix.inverted();
        cameraData.cameraPos = FVector4<float>(cameraPosition, 0.0f);
        cameraData.cameraDir = FVector4<float>(cameraDirection, 0.0f);

        void* mappedData = m_CameraBuffer->map();
        memcpy(mappedData, &cameraData, sizeof(FSceneData));
        m_CameraBuffer->unmap(mappedData);
        m_Begin = false;
    }

    void FSceneRenderer::render(ICommandBuffer* cmdBuffer)
    {
        FRenderPassAttachment baseColorAttachment;
        baseColorAttachment.type = ERenderPassAttachmentType::Color;
        baseColorAttachment.loadOp = ELoadOp::Clear;
        baseColorAttachment.storeOp = EStoreOp::Store;
        baseColorAttachment.clearValue.color = FColor::Black;

        FRenderPassAttachment albedoAttachment = baseColorAttachment;
        albedoAttachment.textureView = m_GBuffer->getAlbedoTexture()->getTextureView();

        FRenderPassAttachment metallicRoughnessAttachment = baseColorAttachment;
        metallicRoughnessAttachment.textureView = m_GBuffer->getMetallicRoughnessTexture()->getTextureView();

        FRenderPassAttachment normalAttachment = baseColorAttachment;
        normalAttachment.textureView = m_GBuffer->getAlbedoTexture()->getTextureView();

        FRenderPassAttachment emissionAttachment = baseColorAttachment;
        emissionAttachment.textureView = m_GBuffer->getEmissionTexture()->getTextureView();

        FRenderPassAttachment positionAttachment = baseColorAttachment;
        positionAttachment.textureView = m_GBuffer->getPositionTexture()->getTextureView();

        FRenderPassAttachment depthStencilAttachment;
        depthStencilAttachment.type = ERenderPassAttachmentType::DepthStencil;
        depthStencilAttachment.loadOp = ELoadOp::Clear;
        depthStencilAttachment.storeOp = EStoreOp::Store;
        depthStencilAttachment.clearValue.depth = 1.0f;
        depthStencilAttachment.clearValue.stencil = 0;
        depthStencilAttachment.textureView = m_GBuffer->getDepthStencilTexture()->getTextureView();

        FRenderPassDesc renderPassDesc;
        renderPassDesc.renderArea = {0, 0, m_Width, m_Height};
        renderPassDesc.colorAttachments.add(&albedoAttachment);
        renderPassDesc.colorAttachments.add(&metallicRoughnessAttachment);
        renderPassDesc.colorAttachments.add(&normalAttachment);
        renderPassDesc.colorAttachments.add(&emissionAttachment);
        renderPassDesc.colorAttachments.add(&positionAttachment);
        renderPassDesc.depthStencilAttachment = &depthStencilAttachment;

        cmdBuffer->beginDebugGroup("Deferred Renderer [Geometry Pass]", FColor::Blue);
        cmdBuffer->beginRenderPass(renderPassDesc);
        cmdBuffer->setViewport(FViewport(0, 0, m_Width, m_Height, 0.0f, 1.0f));
        cmdBuffer->setScissor(FScissor(0, 0, m_Width, m_Height));
        cmdBuffer->bindGraphicsPipeline(m_GeometryPipeline);
        cmdBuffer->bindBindingSet(m_BindingSet1, m_GeometryPassShader);
        cmdBuffer->bindBindingSet(m_BindingSet2, m_GeometryPassShader);

        if (!m_Scene)
        {
            cmdBuffer->endRenderPass();
            cmdBuffer->endDebugGroup();
            return;
        }

        for (const FEntity* entity : m_Scene->getEntities())
        {
            TArray<FStaticMeshRenderer*> renderers = entity->getAllComponentsOfType<FStaticMeshRenderer>();
            if (renderers.isEmpty()) continue;

            for (FStaticMeshRenderer* renderer : renderers)
            {
                Ref<FStaticMesh> staticMesh = renderer->getMesh();
                if (!staticMesh) continue;

                const auto& materialSlots = staticMesh->getMaterialSlots();
                const auto& perMaterialMeshParts = staticMesh->getPerMaterialMeshParts();

                WeakRef<IBuffer> vertexBuffer = staticMesh->getVertexBuffer();
                if (!vertexBuffer) continue;

                WeakRef<IBuffer> indexBuffer = staticMesh->getIndexBuffer();
                if (!indexBuffer) continue;

                for (const auto& [index, slot] : materialSlots)
                {
                    Ref<FMaterial> material = slot.material;
                    if (!material) continue;

                    cmdBuffer->bindMaterial(material);
                    const TArray<FMeshPart>& meshParts = perMaterialMeshParts[index];
                    for (const FMeshPart& meshPart : meshParts)
                    {
                        cmdBuffer->bindVertexBuffer(vertexBuffer, meshPart.vertexOffset);
                        cmdBuffer->bindIndexBuffer(indexBuffer, meshPart.indexOffset, EIndexFormat::Uint32);
                        cmdBuffer->drawIndexed(FDrawIndexedCommand(meshPart.indexSize / sizeof(uint32_t), 1, 0, 0, 0));
                    }
                }
            }
        }

        cmdBuffer->endRenderPass();
        cmdBuffer->endDebugGroup();
    }
}
