#pragma once
#include "Window.h"
#include "Luma/Core/Export.h"

namespace luma
{
    class LUMA_CORE_API FDesktopWindow : public IWindow
    {
    public:
        FDesktopWindow();
        ~FDesktopWindow() override;
        bool initialize(const FWindowCreateInfo& createInfo) override;
        void destroy() override;
        void pollEvents() override;
        uint32_t getWidth() const override;
        uint32_t getHeight() const override;
        bool shouldClose() const;
    private:
        Impl* m_Pimpl = nullptr;
    };
}