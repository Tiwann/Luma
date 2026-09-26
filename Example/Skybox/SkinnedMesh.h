#pragma once
#include "Luma/Containers/StringView.h"
#include "Luma/Math/Vector2.h"
#include "Luma/Math/Vector3.h"
#include "Luma/Math/Vector4.h"
#include "Luma/Math/Matrix4.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Rendering/Buffer.h"
#include "Luma/Rendering/Device.h"

namespace Luma
{
    using RHI::Device;
    using RHI::Buffer;

    struct SkinnedVertex
    {
        FVector3f position;
        FVector2f uv;
        FVector3f normal;
        FVector3f tangent;
        FVector4i boneIds;
        FVector4f boneWeights;
        FVector4f color;
    };

    struct BoneInfo
    {
        uint32_t id;
        FMatrix4f localToBone;
    };

    class SkinnedMesh
    {
    public:
        bool loadFromFile(StringView filepath, Device* device);

    private:
        Ref<Buffer> m_VertexBuffer = nullptr;
        Ref<Buffer> m_IndexBuffer = nullptr;
    };
}
