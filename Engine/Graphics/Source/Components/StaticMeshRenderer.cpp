#include "Luma/Components/StaticMeshRenderer.h"

namespace Luma
{
    Ref<FStaticMesh> FStaticMeshRenderer::getMesh() const
    {
         return m_StaticMesh;
    }

    void FStaticMeshRenderer::setMesh(Ref<FStaticMesh> newMesh)
    {
        m_StaticMesh = newMesh;
    }
}
