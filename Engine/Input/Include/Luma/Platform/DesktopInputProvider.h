#pragma once
#include "Luma/Input/InputProvider.h"

namespace Luma
{
    class FDesktopWindow;

    class FDesktopInputProvider : public IInputProvider
    {
    public:
        FDesktopInputProvider(FDesktopWindow* window);
        bool initialize() override;
        void update();
        void destroy() override;
        bool getEvent(FInputEvent& e) override;
    private:
        FDesktopWindow* m_Window = nullptr;
        TFifo<FInputEvent> m_Events;
    };
}