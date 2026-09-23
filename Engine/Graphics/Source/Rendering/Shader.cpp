#include "Luma/Rendering/BindingGroup.h"
#include "Luma/Rendering/Shader.h"
#include "Luma/Rendering/Device.h"

#ifdef LUMA_BUILD_VULKAN
#include "Luma/Vulkan/Shader.h"
#include "Luma/Vulkan/BindingGroup.h"
#endif

namespace Luma::RHI
{
    BindingGroup* Shader::createBindingGroup(uint32_t groupIndex)
    {
        switch (m_Device->getDeviceType())
        {
#ifdef LUMA_BUILD_VULKAN
        case DeviceType::Vulkan:
            return new Vulkan::BindingGroup(static_cast<Vulkan::Shader*>(this), groupIndex);
#endif
        default: return nullptr;
        }
    }
}
