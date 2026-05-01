#include "Luma/Platform/DesktopInputProvider.h"
#include "Luma/Containers/HashMap.h"
#include "Luma/Runtime/Time.h"
#include "Luma/Runtime/DesktopWindow.h"
#include <rgfw/rgfw.h>


namespace Luma
{
    static EKeyCode getKeyCode(RGFW_key key)
    {
        switch (key)
        {
        case RGFW_keyNULL: return EKeyCode::None;

        case RGFW_keyA: return EKeyCode::A;
        case RGFW_keyB: return EKeyCode::B;
        case RGFW_keyC: return EKeyCode::C;
        case RGFW_keyD: return EKeyCode::D;
        case RGFW_keyE: return EKeyCode::E;
        case RGFW_keyF: return EKeyCode::F;
        case RGFW_keyG: return EKeyCode::G;
        case RGFW_keyH: return EKeyCode::H;
        case RGFW_keyI: return EKeyCode::I;
        case RGFW_keyJ: return EKeyCode::J;
        case RGFW_keyK: return EKeyCode::K;
        case RGFW_keyL: return EKeyCode::L;
        case RGFW_keyM: return EKeyCode::M;
        case RGFW_keyN: return EKeyCode::N;
        case RGFW_keyO: return EKeyCode::O;
        case RGFW_keyP: return EKeyCode::P;
        case RGFW_keyQ: return EKeyCode::Q;
        case RGFW_keyR: return EKeyCode::R;
        case RGFW_keyS: return EKeyCode::S;
        case RGFW_keyT: return EKeyCode::T;
        case RGFW_keyU: return EKeyCode::U;
        case RGFW_keyV: return EKeyCode::V;
        case RGFW_keyW: return EKeyCode::W;
        case RGFW_keyX: return EKeyCode::X;
        case RGFW_keyY: return EKeyCode::Y;
        case RGFW_keyZ: return EKeyCode::Z;

        case RGFW_key0: return EKeyCode::Numpad0;
        case RGFW_key1: return EKeyCode::Numpad1;
        case RGFW_key2: return EKeyCode::Numpad2;
        case RGFW_key3: return EKeyCode::Numpad3;
        case RGFW_key4: return EKeyCode::Numpad4;
        case RGFW_key5: return EKeyCode::Numpad5;
        case RGFW_key6: return EKeyCode::Numpad6;
        case RGFW_key7: return EKeyCode::Numpad7;
        case RGFW_key8: return EKeyCode::Numpad8;
        case RGFW_key9: return EKeyCode::Numpad9;

        case RGFW_keySpace: return EKeyCode::Space;
        case RGFW_keyReturn: return EKeyCode::Enter;
        case RGFW_keyBackSpace: return EKeyCode::Backspace;
        case RGFW_keyDelete: return EKeyCode::Delete;

        case RGFW_keyLeft: return EKeyCode::Left;
        case RGFW_keyRight: return EKeyCode::Right;
        case RGFW_keyUp: return EKeyCode::Up;
        case RGFW_keyDown: return EKeyCode::Down;

        case RGFW_keyEscape: return EKeyCode::Escape;
        case RGFW_keyTab: return EKeyCode::Tab;
        case RGFW_keyCapsLock: return EKeyCode::CapsLock;

        case RGFW_keyShiftL: return EKeyCode::LShift;
        case RGFW_keyShiftR: return EKeyCode::RShift;
        case RGFW_keyControlL: return EKeyCode::LCtrl;
        case RGFW_keyControlR: return EKeyCode::RCtrl;
        case RGFW_keyAltL: return EKeyCode::LAlt;
        case RGFW_keyAltR: return EKeyCode::RAlt;

        case RGFW_keyF1: return EKeyCode::F1;
        case RGFW_keyF2: return EKeyCode::F2;
        case RGFW_keyF3: return EKeyCode::F3;
        case RGFW_keyF4: return EKeyCode::F4;
        case RGFW_keyF5: return EKeyCode::F5;
        case RGFW_keyF6: return EKeyCode::F6;
        case RGFW_keyF7: return EKeyCode::F7;
        case RGFW_keyF8: return EKeyCode::F8;
        case RGFW_keyF9: return EKeyCode::F9;
        case RGFW_keyF10: return EKeyCode::F10;
        case RGFW_keyF11: return EKeyCode::F11;
        case RGFW_keyF12: return EKeyCode::F12;

        default: return EKeyCode::None;
        }
    }

    static EMouseCode getMouseCode(RGFW_mouseButton button)
    {
        switch (button)
        {
        case RGFW_mouseLeft: return EMouseCode::Left;
        case RGFW_mouseMiddle: return EMouseCode::Middle;
        case RGFW_mouseRight: return EMouseCode::Right;
        default: return EMouseCode::None;
        }
    }


    FDesktopInputProvider::FDesktopInputProvider(FDesktopWindow* window) : m_Window(window)
    {
    }

    bool FDesktopInputProvider::initialize()
    {
        return true;
    }

    void FDesktopInputProvider::update()
    {
        RGFW_event e;
        while (RGFW_checkEvent(&e))
        {

            if (e.type == RGFW_keyPressed)
            {
                FInputEvent event;
                event.deviceType = EInputDeviceType::Keyboard;
                event.key.type = EInputEventType::Pressed;
                event.key.code = getKeyCode(e.key.value);
                event.key.value = static_cast<float>(e.key.state);
                event.key.time = FTime::getTime();
                m_Events.enqueue(event);
            }
        }
    }

    void FDesktopInputProvider::destroy()
    {
    }

    bool FDesktopInputProvider::getEvent(FInputEvent& e)
    {
        if (!m_Events.isEmpty())
        {
            e = m_Events.dequeue();
            return true;
        }
        return false;
    }
}
