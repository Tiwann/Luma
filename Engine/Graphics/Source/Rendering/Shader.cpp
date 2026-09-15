#include "Luma/Rendering/BindingGroup.h"
#include "Luma/Rendering/Shader.h"
#include "Luma/Rendering/GPUDevice.h"

#ifdef LUMA_BUILD_VULKAN
#include "Luma/Vulkan/ShaderImpl.h"
#include "Luma/Vulkan/BindingGroupImpl.h"
#endif

namespace Luma
{
    IBindingGroup* IShader::createBindingGroup(uint32_t groupIndex)
    {
        switch (m_Device->getDeviceType())
        {
#ifdef LUMA_BUILD_VULKAN
        case EGPUDeviceType::Vulkan:
            return new Vulkan::FBindingGroupImpl(static_cast<Vulkan::FShaderImpl*>(this), groupIndex);
#endif
        default: return nullptr;
        }
    }
}
