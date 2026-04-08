#include "Modules/ModuleManager.h"

namespace Luma
{
    IModule* FModuleManager::GetModule(const std::wstring& name)
    {
        const auto it = m_ModuleMap.find(name);
        if (it == m_ModuleMap.end())
            return nullptr;
        return it->second;
    }
}

