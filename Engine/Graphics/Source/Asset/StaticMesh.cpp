#include "Luma/Asset/StaticMesh.h"
#include "Luma/Asset/MeshVertex.h"
#include "Luma/Containers/BufferView.h"
#include "Luma/Math/Vector2.h"
#include "Luma/Math/Vector3.h"
#include "Luma/Math/Vector4.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/GltfMaterial.h>

#include "Luma/Rendering/BufferUtils.h"
#include "Luma/Rendering/TextureUtils.h"


namespace Luma
{
    static TArray<uint32_t> getIndicesFromFaces(const TBufferView<aiFace>& faces)
    {
        TArray<uint32_t> result;
        for (uint32_t faceIndex = 0; faceIndex < faces.count(); ++faceIndex)
        {
            const aiFace& face = faces[faceIndex];
            result.addRange(face.mIndices, face.mNumIndices);
        }
        return result;
    }

    static TArray<MeshVertex> getVerticesFromMesh(const aiMesh& mesh)
    {
        TArray<MeshVertex> result;

        const auto toVector3 = [](const aiVector3D& in) { return FVector3<float>(in.x, in.y, in.z); };
        const auto toVector2 = [](const aiVector3D& in) { return FVector2<float>(in.x, in.y); };
        const auto toVector4 = [](const aiColor4D& in) { return FVector4<float>(in.r, in.g, in.b, in.a); };

        for (uint32_t vertexIndex = 0; vertexIndex < mesh.mNumVertices; ++vertexIndex)
        {
            const aiVector3D& position = mesh.HasPositions() ? mesh.mVertices[vertexIndex] : aiVector3D(0, 0, 0);
            const aiVector3D& texCoord = mesh.HasTextureCoords(0) ? mesh.mTextureCoords[0][vertexIndex] : aiVector3D(0, 0, 0);
            const aiVector3D& normal = mesh.HasNormals() ? mesh.mNormals[vertexIndex] : aiVector3D(0, 0, 0);
            const aiVector3D& tangent = mesh.HasTangentsAndBitangents() ? mesh.mTangents[vertexIndex] : aiVector3D(0, 0, 0);
            const aiColor4D& color = mesh.HasVertexColors(0) ? mesh.mColors[0][vertexIndex] : aiColor4D(0, 0, 0, 0);

            const MeshVertex vertex
            {
                .position = toVector3(position),
                .texCoords = toVector2(texCoord),
                .normal = toVector3(normal),
                .tangent = toVector3(tangent),
                .color = toVector4(color)
            };

            result.add(vertex);
        }

        return result;
    }

    void FStaticMesh::destroy()
    {
        m_VertexBuffer->destroy();
        m_IndexBuffer->destroy();
    }

    bool FStaticMesh::loadFromFile(FStringView filepath, IRenderDevice* device)
    {
        if (filepath.isEmpty()) return false;
        if (!device) return false;

        Assimp::Importer importer;
        constexpr auto flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals
        | aiProcess_JoinIdenticalVertices | aiProcess_EmbedTextures | aiProcess_PreTransformVertices;

        const aiScene* loadedScene = importer.ReadFile(*filepath, flags);
        if (!loadedScene) return false;
        if (!loadedScene->HasMeshes()) return false;

        TArray<MeshVertex> allVertices;
        TArray<uint32_t> allIndices;
        uint64_t vertexOffset = 0;
        uint64_t indexOffset = 0;

        for (size_t meshIndex = 0; meshIndex < loadedScene->mNumMeshes; meshIndex++)
        {
            const aiMesh* loadedMesh = loadedScene->mMeshes[meshIndex];
            const uint32_t materialSlotIndex = loadedMesh->mMaterialIndex;
            const aiString materialSlotName = loadedScene->mMaterials[materialSlotIndex]->GetName();

            FMaterialSlot& materialSlot = m_MaterialSlots[materialSlotIndex];
            materialSlot.name = FString(materialSlotName.C_Str());

            TArray<uint32_t> indices = getIndicesFromFaces(TBufferView(loadedMesh->mFaces, loadedMesh->mNumFaces));
            TArray<MeshVertex> vertices = getVerticesFromMesh(*loadedMesh);

            allVertices.addRange(vertices);
            allIndices.addRange(indices);

            FMeshPart meshPart { };
            meshPart.vertexSize = vertices.size();
            meshPart.vertexOffset = vertexOffset;
            meshPart.indexSize = indices.size();
            meshPart.indexOffset = indexOffset;
            m_PerMaterialData[materialSlotIndex].add(meshPart);

            vertexOffset += meshPart.vertexSize;
            indexOffset += meshPart.indexSize;
        }

        m_VertexBuffer = BufferUtils::createVertexBuffer(device, allVertices.data(), allVertices.count());
        if (!m_VertexBuffer) return false;

        m_IndexBuffer = BufferUtils::createIndexBuffer(device, allVertices.data(), allVertices.count());
        if (!m_IndexBuffer) return false;

#if 0
        const auto getTexture = [loadedScene, device](const aiMaterial* material, aiTextureType textureType) -> Ref<ITexture>
        {
            aiString path;
            if (material->GetTexture(textureType, 0, &path) != aiReturn_SUCCESS) return nullptr;

            if (const aiTexture* loadedTexture = loadedScene->GetEmbeddedTexture(path.C_Str()))
            {
                if (loadedTexture->mHeight == 0)
                {
                    const uint8_t* data = reinterpret_cast<uint8_t*>(loadedTexture->pcData);
                    const uint64_t size = loadedTexture->mWidth;
                    Ref<ITexture> texture = TextureUtils::loadTexture(device, data, size);
                    return texture;
                }
                else
                {
                    const uint8_t* data = reinterpret_cast<uint8_t*>(loadedTexture->pcData);
                    const uint32_t width = loadedTexture->mWidth;
                    const uint32_t height = loadedTexture->mHeight;
                    const FTextureDesc textureDesc = FTextureDesc::texture2D(width, height, EFormat::R8G8B8A8_UNORM);
                    Ref<ITexture> texture = device->createTexture(textureDesc);
                    if (!texture) return nullptr;

                    if (!TextureUtils::uploadTextureData(device, texture, 0, 0, data, width * height * 4))
                    {
                        texture->destroy();
                        return nullptr;
                    }
                    return texture;
                }
            }

            return nullptr;
        };

        TBufferView<aiMaterial*> materials(loadedScene->mMaterials, loadedScene->mNumMaterials);
        for (auto& [index, slot] : m_MaterialSlots)
        {
            const aiMaterial* loadedMaterial = materials[index];

            FMaterialTextures materialTextures;
            materialTextures.baseColor = getTexture(loadedMaterial, aiTextureType_BASE_COLOR);
            materialTextures.metallicRoughnessAO = getTexture(loadedMaterial, aiTextureType_GLTF_METALLIC_ROUGHNESS);
            materialTextures.emission = getTexture(loadedMaterial, aiTextureType_EMISSION_COLOR);
            materialTextures.normal = getTexture(loadedMaterial, aiTextureType_NORMALS);
            m_Textures[index] = materialTextures;

            FMaterialDesc materialDesc;
            materialDesc.shader = shader;
            slot.material = device->createMaterial(materialDesc);
            slot.material->setTexture("albedoTex", materialTextures.baseColor, EBindingType::SampledTexture);
            slot.material->setTexture("metallicRoughnessTex", materialTextures.metallicRoughnessAO, EBindingType::SampledTexture);
            slot.material->setTexture("normalTex", materialTextures.normal, EBindingType::SampledTexture);
            slot.material->setTexture("emissionTex", materialTextures.emission, EBindingType::SampledTexture);
        }

#endif
        return true;
    }

    void FStaticMesh::setMaterial(uint32_t slot, Ref<FMaterial> material)
    {
        m_MaterialSlots[slot].material = material;
    }

    Ref<FMaterial> FStaticMesh::getMaterial(uint32_t slot)
    {
        return m_MaterialSlots[slot].material;
    }

    WeakRef<IBuffer> FStaticMesh::getVertexBuffer() const
    {
        return m_VertexBuffer;
    }

    WeakRef<IBuffer> FStaticMesh::getIndexBuffer() const
    {
        return m_IndexBuffer;
    }
}
