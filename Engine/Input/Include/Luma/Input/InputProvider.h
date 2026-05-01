#pragma once
#include "InputEvent.h"
#include "Luma/Containers/Fifo.h"


namespace Luma
{
    struct IInputProvider
    {
        virtual ~IInputProvider() = default;

        virtual bool initialize() = 0;
        virtual void destroy() = 0;
        virtual bool getEvent(FInputEvent& e) = 0;
    };
}
