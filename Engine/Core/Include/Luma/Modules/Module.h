#pragma once
#include "Luma/Core/Export.h"
#include <string_view>

namespace luma
{
    enum class EModuleType
    {
        GameModule,
        CodeModule,
    };

    struct LUMA_CORE_API IModule
    {
        virtual ~IModule() = default;
        virtual EModuleType getModuleType() = 0;
        virtual std::wstring_view getModuleName() = 0;
        virtual void startup() = 0;
        virtual void shutdown() = 0;
    };
}
