#pragma once
#include "Window.h"

struct NWindow;

namespace Luma
{
    enum class ESwitchHandleMode
    {
        Handled,
        Docked
    };

    enum class ESwitchState
    {
        Handled,
        Docked
    };

    class FSwitchWindow : public IWindow
    {
    public:
        bool initialize(const FWindowDesc& windowDesc) override;
        void destroy() override;
        void pollEvents() override;
        uint32_t getWidth() const override;
        uint32_t getHeight() const override;
        bool shouldClose() const override;

        NWindow* getHandle() const;
        ESwitchHandleMode getHandleMode() const;
        bool isFocused() const { return m_Focus; }
    private:
        void setHandleMode(ESwitchHandleMode mode);
        void setFocus(bool focus);
        NWindow* m_Handle = nullptr;
        ESwitchHandleMode m_HandleMode = ESwitchHandleMode::Docked;
        uint32_t m_Width = 1920;
        uint32_t m_Height = 1080;
        bool m_Focus = true;
    };
}
