#pragma once
#include <unordered_map>
#include <vector>
#include <string>

namespace luma
{
    struct IModule;

    class FModuleManager
    {
    public:
        FModuleManager() = default;
        ~FModuleManager() = default;

        IModule* GetModule(const std::wstring& name);

        template<typename TModule> requires std::is_base_of_v<IModule, TModule>
        TModule* GetModule(const std::wstring& name)
        {
            return dynamic_cast<TModule*>(GetModule(name));
        }
    private:
        std::vector<IModule*> m_Modules;
        std::unordered_map<std::wstring, IModule*> m_ModuleMap;
    };
}