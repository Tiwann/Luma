#pragma once
#include "Luma/Memory/Ref.h"
#include "Luma/Runtime/Component.h"
#include "Luma/Asset/StaticMesh.h"

namespace Luma
{
    class FStaticMeshRenderer : public Component
    {
    public:
        FStaticMeshRenderer() = default;

        Ref<StaticMesh> getMesh() const;
        void setMesh(Ref<StaticMesh> newMesh);
    private:
        Ref<StaticMesh> m_StaticMesh = nullptr;
    };
}
