#pragma once
#include "Luma/Core/Export.h"
#include "Luma/Containers/Event.h"
#include "Luma/Containers/String.h"
#include "Luma/Memory/RefCounted.h"
#include "Flags.h"
#include <cstdint>

namespace Luma
{
    enum class EWindowCreateBits
    {
        None = 0,
        Centered = BIT(0),
        NoDecoration = BIT(1),
        Transparent = BIT(2),
        FullScreen = BIT(3),
        Resizable = BIT(4),
        NoDragAndDrop = BIT(5),
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
        virtual bool shouldClose() const = 0;

        TEvent<void(uint32_t, uint32_t)> resizedEvent;
        TEvent<void(uint32_t, uint32_t)> movedEvent;
        TEvent<void()> closedEvent;
        TEvent<void()> minimizedEvent;
        TEvent<void()> maximizedEvent;
        TEvent<void(bool)> focusedEvent;
    };

    LUMA_CORE_API IWindow* createWindow(const FWindowDesc& windowDesc);
}
