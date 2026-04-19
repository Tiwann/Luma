#pragma once
#include "Luma/Containers/String.h"
#include <cstdint>

namespace Luma
{
    struct FWindowDesc
    {
        FString title;
        uint32_t width;
        uint32_t height;
        uint32_t flags;
    };

    struct IWindow
    {
        virtual ~IWindow() = default;
        virtual bool initialize(const FWindowDesc& windowDesc) = 0;
        virtual void destroy() = 0;
        virtual void pollEvents() = 0;
        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getHeight() const = 0;
    };
}
