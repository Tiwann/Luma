#pragma once
#include "Luma/Memory/Ref.h"
#include "Luma/Runtime/Component.h"
#include "Luma/Asset/StaticMesh.h"

namespace Luma
{
    class FStaticMeshRenderer : public IComponent
    {
    public:
        FStaticMeshRenderer() = default;

        Ref<FStaticMesh> getMesh() const;
        void setMesh(Ref<FStaticMesh> newMesh);
    private:
        Ref<FStaticMesh> m_StaticMesh = nullptr;
    };
}
