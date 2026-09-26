#include "SkinnedMesh.h"
#include "Luma/Containers/BufferView.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/GltfMaterial.h>

#include "Luma/Containers/HashMap.h"

namespace Luma
{

    static constexpr uint32_t kMaxBoneInfluence = 4;

    static Array<uint32_t> getIndicesFromFaces(const BufferView<aiFace>& faces)
    {
        Array<uint32_t> result;
        for (const aiFace& face : faces)
            result.addRange(face.mIndices, face.mNumIndices);
        return result;
    }

    static Array<SkinnedVertex> getVerticesFromMesh(const aiMesh& mesh)
    {
        Array<SkinnedVertex> result;

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

            const SkinnedVertex vertex
            {
                .position = toVector3(position),
                .uv = toVector2(texCoord),
                .normal = toVector3(normal),
                .tangent = toVector3(tangent),
                .boneIds = FVector4i(-1),
                .boneWeights = FVector4f(),
                .color = toVector4(color),
            };

            result.add(vertex);
        }

        return result;
    }

    static void extractBones(Array<SkinnedVertex>& vertices, const BufferView<aiBone*>& bones)
    {
        HashMap<String, BoneInfo> boneInfos;
        uint32_t boneCounter = 0;
        uint32_t boneId;

        const auto toMatrix4 = [](const aiMatrix4x4& matrix) -> FMatrix4f
        {
            FMatrix4f result;
            Memory::memcpy(result.valuePtr(), &matrix, sizeof(aiMatrix4x4));
            return result.transposed();
        };

        for (const aiBone* bone : bones)
        {
            String boneName = bone->mName.C_Str();

            if (!boneInfos.find(boneName))
            {
                BoneInfo newBoneInfo;
                newBoneInfo.id = boneCounter;
                newBoneInfo.localToBone = toMatrix4(bone->mOffsetMatrix);
                boneInfos[boneName] = newBoneInfo;
                boneId = boneCounter;
                boneCounter++;
            }
            else
            {
                boneId = boneInfos[boneName].id;
            }

            const BufferView<aiVertexWeight> boneWeights = BufferView(bone->mWeights, bone->mNumWeights);

            for (const aiVertexWeight& weight : boneWeights)
            {
                const uint32_t vertexId = weight.mVertexId;

                for (uint32_t slot = 0; slot < kMaxBoneInfluence; ++slot)
                {
                    // Find first empty slot (-1) for this vertex.
                    if (vertices[vertexId].boneIds[slot] < 0)
                    {
                        vertices[vertexId].boneWeights[slot] = weight.mWeight;
                        vertices[vertexId].boneIds[slot] = boneId;
                        break;
                    }
                }
            }
        }
    }

    bool SkinnedMesh::loadFromFile(StringView filepath, Device* device)
    {
        if (filepath.isEmpty()) return false;
        if (!device) return false;

        Assimp::Importer importer;
        constexpr auto flags = aiProcess_Triangulate;

        const aiScene* loadedScene = importer.ReadFile(*filepath, flags);
        if (!loadedScene) return false;
        if (!loadedScene->HasMeshes()) return false;

        Array<SkinnedVertex> allVertices;
        Array<uint32_t> allIndices;
        uint64_t vertexOffset = 0;
        uint64_t indexOffset = 0;

        for (size_t meshIndex = 0; meshIndex < loadedScene->mNumMeshes; meshIndex++)
        {
            const aiMesh* loadedMesh = loadedScene->mMeshes[meshIndex];

            Array<uint32_t> indices = getIndicesFromFaces(BufferView(loadedMesh->mFaces, loadedMesh->mNumFaces));
            Array<SkinnedVertex> vertices = getVerticesFromMesh(*loadedMesh);
            extractBones(vertices, BufferView(loadedMesh->mBones, loadedMesh->mNumBones));

            allVertices.addRange(std::move(vertices));
            allIndices.addRange(std::move(indices));

            /*MeshPart meshPart { };
            meshPart.vertexSize = vertices.size();
            meshPart.vertexOffset = vertexOffset;
            meshPart.indexSize = indices.size();
            meshPart.indexOffset = indexOffset;
            m_PerMaterialData[materialSlotIndex].add(meshPart);
            vertexOffset += meshPart.vertexSize;
            indexOffset += meshPart.indexSize;*/
        }
        return false;
    }
}
