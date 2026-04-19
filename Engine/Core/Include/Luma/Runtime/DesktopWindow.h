#pragma once
#include "Window.h"
#include "Luma/Containers/String.h"
#include "Luma/Core/Export.h"
#include "Luma/Math/Vector.h"

struct RGFW_window;

namespace Luma
{
    class LUMA_CORE_API FDesktopWindow : public IWindow
    {
    public:
        FDesktopWindow() = default;
        ~FDesktopWindow() override = default;

        bool initialize(const FWindowDesc& windowDesc) override;
        void destroy() override;
        void pollEvents() override;
        uint32_t getWidth() const override;
        uint32_t getHeight() const override;
        FVector2u getPosition() const;
        void setPosition(const FVector2u& position);
        bool hasFocus() const;
        bool isMaximized() const;
        bool isMinimized() const;
        void setFullscreen(bool fullscreen);
        bool shouldClose() const;

        RGFW_window* getHandle() const;

        FString getTitle() const;
        void setTitle(const FString& title);
    private:
        RGFW_window* m_Handle = nullptr;
    };
}
