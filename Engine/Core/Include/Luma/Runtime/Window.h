#pragma once
#include "Luma/Core/Export.h"
#include "Luma/Containers/Event.h"
#include "Luma/Containers/String.h"
#include "Luma/Memory/RefCounted.h"
#include "Luma/Rendering/DeviceType.h"
#include "Luma/Math/Rect2.h"
#include "Flags.h"
#include <cstdint>


namespace Luma
{
    enum class WindowOptions
    {
        None = 0,
        Centered = BIT(0),
        NoDecoration = BIT(1),
        Transparent = BIT(2),
        FullScreen = BIT(3),
        Resizable = BIT(4),
        NoDragAndDrop = BIT(5),
    };

    using WindowOptionsFlags = TFlags<WindowOptions>;


    struct WindowDesc
    {
        FString title;
        uint32_t width = 0;
        uint32_t height = 0;
        WindowOptionsFlags options = 0;
        DeviceType deviceType = DeviceType::None;
    };

    struct Window : RefCounted<Window>
    {
        ~Window() override = default;
        virtual bool initialize(const WindowDesc& windowDesc) = 0;
        virtual void destroy() = 0;
        virtual void pollEvents() = 0;
        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getHeight() const = 0;
        FVector2u getSize() const;
        FRect2<uint32_t> getBounds() const { return FRect2<uint32_t>{ 0, 0, getWidth(), getHeight() }; }
        virtual bool shouldClose() const = 0;
        virtual bool isAvailable() const = 0;
        virtual void setTitle(const FString& title){}

        TEvent<void(uint32_t, uint32_t)> resizedEvent;
        TEvent<void(uint32_t, uint32_t)> movedEvent;
        TEvent<void()> closedEvent;
        TEvent<void()> minimizedEvent;
        TEvent<void()> maximizedEvent;
        TEvent<void(bool)> focusedEvent;
    };

    /// Creates a window, choose the right implementation based on platform
    /// @return A pointer to a IWindow object. Needs to be freed. Consider using Luma::Ref<>
    LUMA_CORE_API Window* createWindow(const WindowDesc& windowDesc);

    /// Creates a window, choose the right implementation based on platform
    /// @return A pointer to a IWindow object. Needs to be freed. Consider using Luma::Ref<>
    LUMA_CORE_API Window* createWindow(const FString& title, uint32_t width, uint32_t height, WindowOptionsFlags options = WindowOptions::None, DeviceType deviceType = DeviceType::Auto);
}
