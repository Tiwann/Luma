#pragma once
#include "Luma/Core/Export.h"
#include "Luma/Containers/String.h"
#include "Luma/Memory/RefCounted.h"
#include <cstdint>

#include "Flags.h"

namespace Luma
{
    enum class EWindowCreateBits
    {
        Centered,
        NoDecoration,
        Transparent,
    };

    using FWindowCreateFlags = TFlags<EWindowCreateBits>;


    struct FWindowDesc
    {
        FString title;
        uint32_t width;
        uint32_t height;
        FWindowCreateFlags flags;
    };

    struct IWindow : IRefCounted<IWindow>
    {
        ~IWindow() override = default;
        virtual bool initialize(const FWindowDesc& windowDesc) = 0;
        virtual void destroy() = 0;
        virtual void pollEvents() = 0;
        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getHeight() const = 0;
    };

    LUMA_CORE_API IWindow* createWindow(const FWindowDesc& windowDesc);
}
