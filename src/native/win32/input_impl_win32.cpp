#include "../../../inc/native/win32/input_impl_win32.h"

#ifdef JOSZVA_SYS_WINDOWS
namespace joszva::engine::priv
{
    namespace input_impl_win32
    {
        bool is_key_pressed(keyboard::key key)
        {
            int vkey = 0;
            switch (key)
            {
            case keyboard::key::A: vkey = 'A'; break;
            case keyboard::key::B: vkey = 'B'; break;
            case keyboard::key::C: vkey = 'C'; break;
            case keyboard::key::D: vkey = 'D'; break;
            case keyboard::key::E: vkey = 'E'; break;
            case keyboard::key::F: vkey = 'F'; break;
            case keyboard::key::G: vkey = 'G'; break;
            case keyboard::key::H: vkey = 'H'; break;
            case keyboard::key::I: vkey = 'I'; break;
            case keyboard::key::J: vkey = 'J'; break;
            case keyboard::key::K: vkey = 'K'; break;
            case keyboard::key::L: vkey = 'L'; break;
            case keyboard::key::M: vkey = 'M'; break;
            case keyboard::key::N: vkey = 'N'; break;
            case keyboard::key::O: vkey = 'O'; break;
            case keyboard::key::P: vkey = 'P'; break;
            case keyboard::key::Q: vkey = 'Q'; break;
            case keyboard::key::R: vkey = 'R'; break;
            case keyboard::key::S: vkey = 'S'; break;
            case keyboard::key::T: vkey = 'T'; break;
            case keyboard::key::U: vkey = 'U'; break;
            case keyboard::key::V: vkey = 'V'; break;
            case keyboard::key::W: vkey = 'W'; break;
            case keyboard::key::X: vkey = 'X'; break;
            case keyboard::key::Y: vkey = 'Y'; break;
            case keyboard::key::Z: vkey = 'Z'; break;
            case keyboard::key::NUM0: vkey = '0'; break;
            case keyboard::key::NUM1: vkey = '1'; break;
            case keyboard::key::NUM2: vkey = '2'; break;
            case keyboard::key::NUM3: vkey = '3'; break;
            case keyboard::key::NUM4: vkey = '4'; break;
            case keyboard::key::NUM5: vkey = '5'; break;
            case keyboard::key::NUM6: vkey = '6'; break;
            case keyboard::key::NUM7: vkey = '7'; break;
            case keyboard::key::NUM8: vkey = '8'; break;
            case keyboard::key::NUM9: vkey = '9'; break;
            case keyboard::key::NUMPAD0: vkey = VK_NUMPAD0; break;
            case keyboard::key::NUMPAD1: vkey = VK_NUMPAD1; break;
            case keyboard::key::NUMPAD2: vkey = VK_NUMPAD2; break;
            case keyboard::key::NUMPAD3: vkey = VK_NUMPAD3; break;
            case keyboard::key::NUMPAD4: vkey = VK_NUMPAD4; break;
            case keyboard::key::NUMPAD5: vkey = VK_NUMPAD5; break;
            case keyboard::key::NUMPAD6: vkey = VK_NUMPAD6; break;
            case keyboard::key::NUMPAD7: vkey = VK_NUMPAD7; break;
            case keyboard::key::NUMPAD8: vkey = VK_NUMPAD8; break;
            case keyboard::key::NUMPAD9: vkey = VK_NUMPAD9; break;
            case keyboard::key::F1: vkey = VK_F1; break;
            case keyboard::key::F2: vkey = VK_F2; break;
            case keyboard::key::F3: vkey = VK_F3; break;
            case keyboard::key::F4: vkey = VK_F4; break;
            case keyboard::key::F5: vkey = VK_F5; break;
            case keyboard::key::F6: vkey = VK_F6; break;
            case keyboard::key::F7: vkey = VK_F7; break;
            case keyboard::key::F8: vkey = VK_F8; break;
            case keyboard::key::F9: vkey = VK_F9; break;
            case keyboard::key::F10: vkey = VK_F10; break;
            case keyboard::key::F11: vkey = VK_F11; break;
            case keyboard::key::F12: vkey = VK_F12; break;
            case keyboard::key::L_SYSTEM: vkey = VK_LWIN; break;
            case keyboard::key::R_SYSTEM: vkey = VK_RWIN; break;
            case keyboard::key::TAB: vkey = VK_TAB; break;
            case keyboard::key::CAPS_LOCK: vkey = VK_CAPITAL; break;
            case keyboard::key::ESCAPE: vkey = VK_ESCAPE; break;
            case keyboard::key::NUMLOCK: vkey = VK_NUMLOCK; break;
            case keyboard::key::DIVIDE: vkey = VK_DIVIDE; break;
            case keyboard::key::MULTIPLY: vkey = VK_MULTIPLY; break;
            case keyboard::key::SUBTRACT: vkey = VK_SUBTRACT; break;
            case keyboard::key::ADD: vkey = VK_ADD; break;
            case keyboard::key::ENTER: vkey = VK_RETURN; break;
            case keyboard::key::DECIMAL: vkey = VK_DECIMAL; break;
            case keyboard::key::UP: vkey = VK_UP; break;
            case keyboard::key::DOWN: vkey = VK_DOWN; break;
            case keyboard::key::LEFT: vkey = VK_LEFT; break;
            case keyboard::key::RIGHT: vkey = VK_RIGHT; break;
            case keyboard::key::INSERT: vkey = VK_INSERT; break;
            case keyboard::key::DEL: vkey = VK_DELETE; break;
            case keyboard::key::SCROLL_LOCK: vkey = VK_SCROLL; break;
            case keyboard::key::HOME: vkey = VK_HOME; break;
            case keyboard::key::END: vkey = VK_END; break;
            case keyboard::key::PAUSE: vkey = VK_PAUSE; break;
            case keyboard::key::PAGE_UP: vkey = VK_PRIOR; break;
            case keyboard::key::PAGE_DOWN: vkey = VK_NEXT; break;
            case keyboard::key::PERIOD: vkey = VK_OEM_PERIOD; break;
            case keyboard::key::COMMA: vkey = VK_OEM_COMMA; break;
            case keyboard::key::QUOTE: vkey = VK_OEM_7; break;
            case keyboard::key::BACKSPACE: vkey = VK_BACK; break;
            case keyboard::key::L_BRACKET: vkey = VK_OEM_4; break;
            case keyboard::key::R_BRACKET: vkey = VK_OEM_6; break;
            case keyboard::key::SLASH: vkey = VK_OEM_2; break;
            case keyboard::key::BACKSLASH: vkey = VK_OEM_5; break;
            case keyboard::key::SEMICOLON: vkey = VK_OEM_1; break;
            case keyboard::key::HYPHEN: vkey = VK_OEM_MINUS; break;
            case keyboard::key::EQUAL: vkey = VK_OEM_PLUS; break;
            case keyboard::key::MENU: vkey = VK_MENU; break;
            case keyboard::key::TILDE: vkey = VK_OEM_3; break;
            }

            return (GetAsyncKeyState(vkey) & 0x8000) != 0;
        }

        bool is_mouse_pressed(mouse::button btn)
        {
            int vkey = 0;
            switch (btn)
            {
                case mouse::button::LEFT: vkey = GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON; break;
                case mouse::button::MIDDLE: vkey = VK_MBUTTON; break;
                case mouse::button::RIGHT: vkey = GetSystemMetrics(SM_SWAPBUTTON) ? VK_LBUTTON : VK_RBUTTON; break;
                case mouse::button::EXTRA1: vkey = VK_XBUTTON1; break;
                case mouse::button::EXTRA2: vkey = VK_XBUTTON2; break;
                default: vkey = 0; break;
            }

            return (GetAsyncKeyState(vkey) & 0x8000) != 0;
        }

        const vector2<int> get_cursor_coords()
        {
            POINT point = {};
            GetCursorPos(&point);
            return vector2<int>(point.x, point.y);
        }

        const vector2<int> get_cursor_coords(const window_impl_win32& win)
        {
            POINT point = {};
            GetCursorPos(&point);
            /* convert screen coords to client coords */
            ScreenToClient(win.get_handle(), &point);
            return vector2<int>(point.x, point.y);
        }

        void set_cursor_coords(const window_impl_win32& win, const vector2<int>& point)
        {
            POINT _point = { point.x, point.y };
            ClientToScreen(win.get_handle(), &_point);
            SetCursorPos(point.x, point.y);
        }
    }
}

#endif