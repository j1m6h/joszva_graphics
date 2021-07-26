#include <thread>

#include "../../../inc/base/event_system/event_dispatcher.h"
#include "../../../inc/base/event_system/event_types.h"

#include "../../../inc/native/win32/window_impl_win32.h"

#ifdef JOSZVA_SYS_WINDOWS
using joszva::graphics::vector2;

using joszva::graphics::priv::window_impl_win32;

window_impl_win32::window_impl_win32(const std::string& title, int width, int height)
    : handle(0),
    title(title),
    callback(0),
    mouse_inside(false)
{
    init_win32(title, width, height);
}

window_impl_win32::~window_impl_win32()
{
    set_mouse_tracking(false);
}

const HWND window_impl_win32::get_handle() const
{
    return handle;
}

void window_impl_win32::set_visible(bool visible)
{
    if (visible)
    {
        ShowWindow(handle, SW_SHOW);
    }
    else 
    {
        ShowWindow(handle, SW_HIDE);
    }
}

const std::string& window_impl_win32::get_title() const
{
    return title;
}

void window_impl_win32::set_title(const std::string& title)
{
    SetWindowText(handle, title.c_str());
    this->title = title;
}

vector2<int> window_impl_win32::get_size() const
{
    RECT rect;
    GetClientRect(handle, &rect);
    return vector2<int>((rect.right - rect.left), (rect.bottom - rect.top));
}

void window_impl_win32::set_size(const vector2<int>& size)
{
}

void window_impl_win32::init_win32(const std::string& title, int width, int height)
{
    WNDCLASSEX class_ex;
    class_ex.cbClsExtra = 0;
    class_ex.cbSize = sizeof(WNDCLASSEX);
    class_ex.cbWndExtra = 0;
    class_ex.hbrBackground = (HBRUSH)CreateSolidBrush(WHITE_BRUSH);
    class_ex.hCursor = 0;
    class_ex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    class_ex.hIconSm = class_ex.hIcon;
    class_ex.hInstance = GetModuleHandle(nullptr);
    class_ex.lpfnWndProc = &initial_procedure;
    class_ex.lpszClassName = (LPCSTR)"Joszva_Window";
    class_ex.lpszMenuName = nullptr;
    class_ex.style = 0;
    RegisterClassEx(&class_ex);

    handle = CreateWindowEx(0, "Joszva_Window", title.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, GetModuleHandle(nullptr), this);

    /* center the window */
    RECT _rect;
    GetWindowRect(handle, &_rect);

    int x = (GetSystemMetrics(SM_CXSCREEN) - _rect.right) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - _rect.bottom) / 2;
    SetWindowPos(handle, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

    ShowWindow(handle, SW_SHOW);
    UpdateWindow(handle);
}

void window_impl_win32::process_events()
{
    if (!callback)
    {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

void window_impl_win32::process_event(UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (handle == nullptr)
    {
        return;
    }

    switch (msg)
    {
        case WM_DESTROY:
        {
            close_event _event;
            event_dispatcher<close_event>::instance()->dispatch(_event);
            break;
        }
        case WM_SIZE:
        {
            RECT client;
            GetClientRect(handle, &client);

            size_event _event;
            _event.width = (client.right - client.left);
            _event.height = (client.bottom - client.top);
            event_dispatcher<size_event>::instance()->dispatch(_event);
            break;
        }
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            keyboard::key k = virtual_to_key(wparam, lparam);

            key_pressed_event _event;
            _event.key = k;
            event_dispatcher<key_pressed_event>::instance()->dispatch(_event);
            break;
        }
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            keyboard::key k = virtual_to_key(wparam, lparam);

            key_released_event _event;
            _event.key = k;
            event_dispatcher<key_released_event>::instance()->dispatch(_event);
            break;
        }
        case WM_MOUSEMOVE:
        {
            RECT client;
            GetClientRect(handle, &client);

            int x = static_cast<int16_t>(LOWORD(lparam));
            int y = static_cast<int16_t>(HIWORD(lparam));

            if ((x < client.left) || (x > client.right) || (y < client.top) || (y > client.bottom))
            {
                if (mouse_inside)
                {
                    mouse_inside = false;

                    set_mouse_tracking(false);

                    mouse_leave_event _event;
                    event_dispatcher<mouse_leave_event>::instance()->dispatch(_event);
                }
            }
            else 
            {
                if (!mouse_inside)
                {
                    mouse_inside = true;

                    /* check for mouse leaving window */
                    set_mouse_tracking(true);

                    mouse_enter_event _event;
                    event_dispatcher<mouse_enter_event>::instance()->dispatch(_event);
                }
            }

            mouse_motion_event _event;
            _event.x = static_cast<int16_t>(LOWORD(lparam));
            _event.y = static_cast<int16_t>(HIWORD(lparam));
            event_dispatcher<mouse_motion_event>::instance()->dispatch(_event);
            break;
        }
        case WM_LBUTTONDOWN:
        {
            mouse_pressed_event _event;
            _event.btn = mouse::button::LEFT;
            _event.x = static_cast<int16_t>(LOWORD(lparam));
            _event.y = static_cast<int16_t>(HIWORD(lparam));
            event_dispatcher<mouse_pressed_event>::instance()->dispatch(_event);
            break;
        }
        case WM_LBUTTONUP:
        {
            mouse_released_event _event;
            _event.btn = mouse::button::LEFT;
            _event.x = static_cast<int16_t>(LOWORD(lparam));
            _event.y = static_cast<int16_t>(HIWORD(lparam));
            event_dispatcher<mouse_released_event>::instance()->dispatch(_event);
            break;
        }
        case WM_RBUTTONDOWN:
        {
            mouse_pressed_event _event;
            _event.btn = mouse::button::RIGHT;
            _event.x = static_cast<int16_t>(LOWORD(lparam));
            _event.y = static_cast<int16_t>(HIWORD(lparam));
            event_dispatcher<mouse_pressed_event>::instance()->dispatch(_event);
            break;
        }
        case WM_RBUTTONUP:
        {
            mouse_released_event _event;
            _event.btn = mouse::button::RIGHT;
            _event.x = static_cast<int16_t>(LOWORD(lparam));
            _event.y = static_cast<int16_t>(HIWORD(lparam));
            event_dispatcher<mouse_released_event>::instance()->dispatch(_event);
            break;
        }
        case WM_MBUTTONDOWN:
        {
            mouse_pressed_event _event;
            _event.btn = mouse::button::MIDDLE;
            _event.x = static_cast<int16_t>(LOWORD(lparam));
            _event.y = static_cast<int16_t>(HIWORD(lparam));
            event_dispatcher<mouse_pressed_event>::instance()->dispatch(_event);
            break;
        }
        case WM_MBUTTONUP:
        {
            mouse_released_event _event;
            _event.btn = mouse::button::MIDDLE;
            _event.x = static_cast<int16_t>(LOWORD(lparam));
            _event.y = static_cast<int16_t>(HIWORD(lparam));
            event_dispatcher<mouse_released_event>::instance()->dispatch(_event);
            break;
        }
        case WM_MOUSELEAVE:
        {
            if (mouse_inside)
            {
                mouse_inside = false;

                mouse_leave_event _event;
                event_dispatcher<mouse_leave_event>::instance()->dispatch(_event);
            }

            break;
        }
    }
}

void window_impl_win32::set_mouse_tracking(bool tracking)
{
    TRACKMOUSEEVENT mouse_event;
    mouse_event.cbSize = sizeof(TRACKMOUSEEVENT);
    mouse_event.dwFlags = tracking ? TME_LEAVE : TME_CANCEL;
    mouse_event.dwHoverTime = HOVER_DEFAULT;
    mouse_event.hwndTrack = handle;
    TrackMouseEvent(&mouse_event);
}

joszva::engine::keyboard::key window_impl_win32::virtual_to_key(WPARAM key, LPARAM flags)
{
    switch (key)
    {
        /* check if right or left shift */
        case VK_SHIFT:
        {
            static UINT left = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
            UINT scancode = static_cast<UINT>((flags & (0xFF << 16)) >> 16);
            return scancode == left ? keyboard::key::L_SHIFT : keyboard::key::R_SHIFT;
        }

        /* check if right or left menu */
        case VK_MENU: return (HIWORD(flags) & KF_EXTENDED) ? keyboard::key::R_ALT : keyboard::key::L_ALT;
        /* check if right or left ctrl */
        case VK_CONTROL : return (HIWORD(flags) & KF_EXTENDED) ? keyboard::key::R_CTRL : keyboard::key::L_CTRL;

        case 'A': return keyboard::key::A;
        case 'B': return keyboard::key::B;
        case 'C': return keyboard::key::C;
        case 'D': return keyboard::key::D;
        case 'E': return keyboard::key::E;
        case 'F': return keyboard::key::F;
        case 'G': return keyboard::key::G;
        case 'H': return keyboard::key::H;
        case 'I': return keyboard::key::I;
        case 'J': return keyboard::key::J;
        case 'K': return keyboard::key::K;
        case 'L': return keyboard::key::L;
        case 'M': return keyboard::key::M;
        case 'N': return keyboard::key::N;
        case 'O': return keyboard::key::O;
        case 'P': return keyboard::key::P;
        case 'Q': return keyboard::key::Q;
        case 'R': return keyboard::key::R;
        case 'S': return keyboard::key::S;
        case 'T': return keyboard::key::T;
        case 'U': return keyboard::key::U;
        case 'V': return keyboard::key::V;
        case 'W': return keyboard::key::W;
        case 'X': return keyboard::key::X;
        case 'Y': return keyboard::key::Y;
        case 'Z': return keyboard::key::Z;
        case '0': return keyboard::key::NUM0;
        case '1': return keyboard::key::NUM1;
        case '2': return keyboard::key::NUM2;
        case '3': return keyboard::key::NUM3;
        case '4': return keyboard::key::NUM4;
        case '5': return keyboard::key::NUM5;
        case '6': return keyboard::key::NUM6;
        case '7': return keyboard::key::NUM7;
        case '8': return keyboard::key::NUM8;
        case '9': return keyboard::key::NUM9;
        case VK_NUMPAD0: return keyboard::key::NUMPAD0;
        case VK_NUMPAD1: return keyboard::key::NUMPAD1;
        case VK_NUMPAD2: return keyboard::key::NUMPAD2;
        case VK_NUMPAD3: return keyboard::key::NUMPAD3;
        case VK_NUMPAD4: return keyboard::key::NUMPAD4;
        case VK_NUMPAD5: return keyboard::key::NUMPAD5;
        case VK_NUMPAD6: return keyboard::key::NUMPAD6;
        case VK_NUMPAD7: return keyboard::key::NUMPAD7;
        case VK_NUMPAD8: return keyboard::key::NUMPAD8;
        case VK_NUMPAD9: return keyboard::key::NUMPAD9;
        case VK_F1: return keyboard::key::F1;
        case VK_F2: return keyboard::key::F2;
        case VK_F3: return keyboard::key::F3;
        case VK_F4: return keyboard::key::F4;
        case VK_F5: return keyboard::key::F5;
        case VK_F6: return keyboard::key::F6;
        case VK_F7: return keyboard::key::F7;
        case VK_F8: return keyboard::key::F8;
        case VK_F9: return keyboard::key::F9;
        case VK_F10: return keyboard::key::F10;
        case VK_F11: return keyboard::key::F11;
        case VK_F12: return keyboard::key::F12;
        case VK_LWIN: return keyboard::key::L_SYSTEM;
        case VK_RWIN: return keyboard::key::R_SYSTEM;
        case VK_TAB: return keyboard::key::TAB;
        case VK_CAPITAL: return keyboard::key::CAPS_LOCK;
        case VK_ESCAPE: return keyboard::key::ESCAPE;
        case VK_NUMLOCK: return keyboard::key::NUMLOCK;
        case VK_DIVIDE: return keyboard::key::DIVIDE;
        case VK_MULTIPLY: return keyboard::key::MULTIPLY;
        case VK_SUBTRACT: return keyboard::key::SUBTRACT;
        case VK_ADD: return keyboard::key::ADD;
        case VK_RETURN: return keyboard::key::ENTER;
        case VK_DECIMAL: return keyboard::key::DECIMAL;
        case VK_UP: return keyboard::key::UP;
        case VK_DOWN: return keyboard::key::DOWN;
        case VK_LEFT: return keyboard::key::LEFT;
        case VK_RIGHT: return keyboard::key::RIGHT;
        case VK_INSERT: return keyboard::key::INSERT;
        case VK_DELETE: return keyboard::key::DEL;
        case VK_SCROLL: return keyboard::key::SCROLL_LOCK;
        case VK_HOME: return keyboard::key::HOME;
        case VK_END: return keyboard::key::END;
        case VK_PAUSE: return keyboard::key::PAUSE;
        case VK_PRIOR: return keyboard::key::PAGE_UP;
        case VK_NEXT: return keyboard::key::PAGE_DOWN;
        case VK_OEM_PERIOD: return keyboard::key::PERIOD;
        case VK_OEM_COMMA: return keyboard::key::COMMA;
        case VK_OEM_7: return keyboard::key::QUOTE;
        case VK_BACK: return keyboard::key::BACKSPACE;
        case VK_OEM_4: return keyboard::key::L_BRACKET;
        case VK_OEM_6: return keyboard::key::R_BRACKET;
        case VK_OEM_2: return keyboard::key::SLASH;
        case VK_OEM_5: return keyboard::key::BACKSLASH;
        case VK_OEM_1: return keyboard::key::SEMICOLON;
        case VK_OEM_MINUS: return keyboard::key::HYPHEN;
        case VK_OEM_PLUS: return keyboard::key::EQUAL;
        case VK_APPS: return keyboard::key::MENU;
        case VK_OEM_3: return keyboard::key::TILDE;
    }

    return keyboard::key::UNKNOWN;
}

LRESULT CALLBACK window_impl_win32::initial_procedure(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (msg == WM_CREATE)
    {
        LONG_PTR win = (LONG_PTR)reinterpret_cast<CREATESTRUCT*>(lparam)->lpCreateParams;
        SetWindowLongPtr(handle, GWLP_USERDATA, win);
    }

    window_impl_win32* win = reinterpret_cast<window_impl_win32*>(GetWindowLongPtr(handle, GWLP_USERDATA));

    if (win)
    {
        win->process_event(msg, wparam, lparam);
    }

    return DefWindowProc(handle, msg, wparam, lparam);
}
#endif