#pragma once
#include "Window.h"
#include "Luma/Containers/String.h"
#include "Luma/Core/Export.h"
#include "Luma/Math/Vector.h"

struct GLFWwindow;

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
        bool shouldClose() const override;
        uint32_t getWidth() const override;
        uint32_t getHeight() const override;
        FVector2u getPosition() const;
        void setPosition(const FVector2u& position);
        bool hasFocus() const;
        bool isMaximized() const;
        bool isMinimized() const;
        void setFullscreen(bool fullscreen);
        bool isAvailable() const override;

        FString getTitle() const;
        void setTitle(const FString& title);

        GLFWwindow* getHandle() const;
    private:
        GLFWwindow* m_Handle = nullptr;
        FString m_Title;
        bool m_Focused = false;
        bool m_Maximized = false;
        bool m_Minimized = false;
    };
}
