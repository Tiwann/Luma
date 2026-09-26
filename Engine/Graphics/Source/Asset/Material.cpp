#include "Luma/Asset/Material.h"
#include "Luma/Rendering/RenderPipeline.h"
#include "Luma/Rendering/Shader.h"
#include "Luma/Rendering/Texture.h"
#include "Luma/Rendering/Buffer.h"
#include "Luma/Rendering/Sampler.h"
#include "Luma/Rendering/BindingGroup.h"

namespace Luma
{
    static constexpr uint32_t MATERIAL_BINDING_SET = 0;

    Material::Material(Ref<Shader> shader) : m_Shader(shader)
    {
        m_Shader->createBindingGroup(MATERIAL_BINDING_SET);
    }

    Material::~Material()
    {
        m_BindingGroup = nullptr;
        m_Shader = nullptr;
    }
}
