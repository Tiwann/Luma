#include "Luma/Components/StaticMeshRenderer.h"
#include "Luma/Rendering/Shader.h"

LUMA_DEFINE_CLASS(Luma::FStaticMeshRenderer)

namespace Luma
{
    Ref<StaticMesh> FStaticMeshRenderer::getMesh() const
    {
         return m_StaticMesh;
    }

    void FStaticMeshRenderer::setMesh(Ref<StaticMesh> newMesh)
    {
        m_StaticMesh = newMesh;
    }
}
