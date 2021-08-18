#include "../../../inc/base/event_system/event_dispatcher.h"
#include "../../../inc/base/event_system/event_types.h"

#include "../../../inc/native/x11/window_impl_x11.h"

#ifdef JOSZVA_SYS_LINUX

using joszva::graphics::vector2;

using joszva::graphics::priv::window_impl_x11;

window_impl_x11::window_impl_x11(const std::string& title, int width, int height)
    : display(nullptr),
    handle(0),
    title(title),
    key_repeat(true)
{
    init_x11(title, width, height);
}

window_impl_x11::~window_impl_x11()
{
    // destroy everything, and free resources...
    if (handle != 0)
    {
        XDestroyWindow(display, handle);
    }
    if (display != nullptr)
    {
        display_x11::close_display(display);
    }
}

const Window window_impl_x11::get_handle() const 
{
    return handle;
}

void window_impl_x11::set_visible(bool visible)
{
    if (visible)
    {
        XMapWindow(display, handle);
        XFlush(display);
    }
    else 
    {
        XUnmapWindow(display, handle);
        XFlush(display);
    }
}

const std::string& window_impl_x11::get_title() const 
{
    return title;
}

void window_impl_x11::set_title(const std::string& title)
{
    XmbSetWMProperties(display, handle, 
        title.c_str(), title.c_str(), nullptr, 0, nullptr, nullptr, nullptr);

    this->title = title;
}

vector2<int> window_impl_x11::get_size() const 
{
    XWindowAttributes attribs;
    XGetWindowAttributes(display, handle, &attribs);
    
    return vector2<int>(attribs.width, attribs.height);
}

void window_impl_x11::set_size(const vector2<int>& size)
{
    
}

void window_impl_x11::init_x11(const std::string& title, int width, int height)
{
    display = display_x11::open_display();

    int screen = DefaultScreen(display);
    //handle = XCreateSimpleWindow(display, RootWindow(display, screen), 20, 20, 1280, 720, 0, BlackPixel(display, screen), WhitePixel(display, screen));
    handle = XCreateSimpleWindow(display, RootWindow(display, screen), 20, 20, width, height, 0, 0, 0);
    XStoreName(display, handle, title.c_str());

    /* event mask that specifies which events we want to receive */
    long event_mask = EnterWindowMask | LeaveWindowMask | ExposureMask |
        KeyPressMask | KeyReleaseMask |
        ButtonPressMask | ButtonReleaseMask | ButtonMotionMask | PointerMotionMask |
        StructureNotifyMask | SubstructureNotifyMask;

    XSelectInput(display, handle, event_mask);

    set_protocols();
}

void window_impl_x11::set_protocols()
{
    Atom wm_protocols = XInternAtom(display, "WM_PROTOCOLS", False);
    Atom wm_delete = XInternAtom(display, "WM_DELETE_WINDOW", False);

    /* make sure to send an event to ClientMessage */
    XChangeProperty(display, handle, wm_protocols, XA_ATOM, 32, PropModeReplace,
        reinterpret_cast<const unsigned char*>(&wm_delete), 1);
}

Bool check_event(Display*, XEvent* event, XPointer user_data)
{
    return event->xany.window == reinterpret_cast<Window>(user_data);
}

void window_impl_x11::process_events()
{
    XEvent event;

    while (XCheckIfEvent(display, &event, &check_event, reinterpret_cast<XPointer>(handle)))
    {
        events.push_back(event);
    }

    while (!events.empty())
    {
        event = events.front();
        events.pop_front();
        process_event(event);
    }
}

void window_impl_x11::wait_events()
{
    while (!XPending(display))
    {
        wait_for_event(nullptr);
    }

    process_events();
}

void window_impl_x11::set_key_repeat(bool enabled)
{
    key_repeat = enabled;
}

bool window_impl_x11::process_event(XEvent& xevent)
{
    /* detect key repeated events, and only allow for key pressed events to repeat */
    if (xevent.type == KeyRelease)
    {
        auto iter = std::find_if(events.begin(), events.end(),
            key_repeat_finder(xevent.xkey.keycode, xevent.xkey.time));

        if (iter != events.end())
        {
            /* remove key released event */
            if (!key_repeat)
            {
                events.erase(iter);
            }

            return false;
        }
    }

    /* convert x events into events of our own */
    switch (xevent.type)
    {
        case ClientMessage:
        {
            Atom wm_delete = XInternAtom(display, "WM_DELETE_WINDOW", False);
            XSetWMProtocols(display, handle, &wm_delete, True);

            /* window closed event */
            if (xevent.xclient.data.l[0] == wm_delete)
            {
                close_event _event = close_event();
                event_dispatcher<close_event>::instance()->dispatch(_event);
            }

            break;
        }
        case ConfigureNotify:
        {
            size_event _event = size_event();
            _event.x = xevent.xconfigure.x;
            _event.y = xevent.xconfigure.y;
            _event.width = xevent.xconfigure.width;
            _event.height = xevent.xconfigure.height;
            event_dispatcher<size_event>::instance()->dispatch(_event);
            break;
        }
        case KeyPress:
        {   
            KeySym sym = XLookupKeysym(&xevent.xkey, False);
            keyboard::key k = keysym_to_key(sym);

            key_pressed_event _event;
            _event.key = k;
            event_dispatcher<key_pressed_event>::instance()->dispatch(_event);
            break;
        }
        case KeyRelease:
        {
            KeySym sym = XLookupKeysym(&xevent.xkey, False);
            keyboard::key k = keysym_to_key(sym);

            key_released_event _event;
            _event.key = k;
            event_dispatcher<key_released_event>::instance()->dispatch(_event);
            break;
        }
        /* mouse moved */
        case MotionNotify:
        {
            Window root, child;
            int root_x, root_y;
            int win_x, win_y;
            unsigned int btns;
            XQueryPointer(display, handle, &root, &child, &root_x, &root_y, &win_x, &win_y, &btns);

            /* make sure no negative numbers are sent */
            if ((win_x) < 0)
            {
                win_x = 0;
            }
            else if ((win_y) < 0)
            {
                win_y = 0;
            }

            mouse_motion_event _event;
            _event.x = win_x;
            _event.y = win_y;
            event_dispatcher<mouse_motion_event>::instance()->dispatch(_event);
            break;
        }
        case EnterNotify:
        {
            mouse_enter_event _event;
            event_dispatcher<mouse_enter_event>::instance()->dispatch(_event);
            break;
        }
        case LeaveNotify:
        {
            mouse_leave_event _event;
            event_dispatcher<mouse_leave_event>::instance()->dispatch(_event);
            break;
        }
        case ButtonPress:
        {
            mouse_pressed_event _event;
            _event.x = xevent.xbutton.x;
            _event.y = xevent.xbutton.y;

            unsigned int btn = xevent.xbutton.button;
            if ((btn == Button1) || (btn == Button2) || (btn == Button3) || (btn == 8) || (btn == 9))
            {
                switch (btn)
                {
                    case Button1: _event.btn = mouse::button::LEFT; break;
                    case Button2: _event.btn = mouse::button::MIDDLE; break;
                    case Button3: _event.btn = mouse::button::RIGHT; break;
                    case 8: _event.btn = mouse::button::EXTRA1; break;
                    case 9: _event.btn = mouse::button::EXTRA2; break;
                }
            }

            event_dispatcher<mouse_pressed_event>::instance()->dispatch(_event);
            break;
        }
        case ButtonRelease:
        {
            mouse_released_event _event;
            _event.x = xevent.xbutton.x;
            _event.y = xevent.xbutton.y;

            unsigned int btn = xevent.xbutton.button;
            if ((btn == Button1) || (btn == Button2) || (btn == Button3) || (btn == 8) || (btn == 9))
            {
                switch (btn)
                {
                    case Button1: _event.btn = mouse::button::LEFT; break;
                    case Button2: _event.btn = mouse::button::MIDDLE; break;
                    case Button3: _event.btn = mouse::button::RIGHT; break;
                    case 8: _event.btn = mouse::button::EXTRA1; break;
                    case 9: _event.btn = mouse::button::EXTRA2; break;
                }
            }

            event_dispatcher<mouse_released_event>::instance()->dispatch(_event);
            break;
        }
        default:
        {
            break;
        }
    }

    return true;
}

bool window_impl_x11::wait_for_event(double* timeout)
{
    fd_set fds;
    const int fd = ConnectionNumber(display);
    int count = fd + 1;

    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    if (select(count, &fds, nullptr, nullptr, nullptr) != -1 || errno != EINTR)
    {
        return true;
    }

    return false;
}

joszva::graphics::keyboard::key window_impl_x11::keysym_to_key(KeySym sym)
{
    switch (sym)
    {
        case XK_A: return keyboard::key::A;
        case XK_B: return keyboard::key::B;
        case XK_C: return keyboard::key::C;
        case XK_D: return keyboard::key::D;
        case XK_E: return keyboard::key::E;
        case XK_F: return keyboard::key::F;
        case XK_G: return keyboard::key::G;
        case XK_H: return keyboard::key::H;
        case XK_I: return keyboard::key::I;
        case XK_J: return keyboard::key::J;
        case XK_K: return keyboard::key::K;
        case XK_L: return keyboard::key::L;
        case XK_M: return keyboard::key::M;
        case XK_N: return keyboard::key::N;
        case XK_O: return keyboard::key::O;
        case XK_P: return keyboard::key::P;
        case XK_Q: return keyboard::key::Q;
        case XK_R: return keyboard::key::R;
        case XK_S: return keyboard::key::S;
        case XK_T: return keyboard::key::T;
        case XK_U: return keyboard::key::U;
        case XK_V: return keyboard::key::V;
        case XK_W: return keyboard::key::W;
        case XK_X: return keyboard::key::X;
        case XK_Y: return keyboard::key::Y;
        case XK_Z: return keyboard::key::Z;
        case XK_0: return keyboard::key::NUM0;
        case XK_1: return keyboard::key::NUM1;
        case XK_2: return keyboard::key::NUM2;
        case XK_3: return keyboard::key::NUM3;
        case XK_4: return keyboard::key::NUM4;
        case XK_5: return keyboard::key::NUM5;
        case XK_6: return keyboard::key::NUM6;
        case XK_7: return keyboard::key::NUM7;
        case XK_8: return keyboard::key::NUM8;
        case XK_9: return keyboard::key::NUM9;
        case XK_KP_0: return keyboard::key::NUMPAD0;
        case XK_KP_1: return keyboard::key::NUMPAD1;
        case XK_KP_2: return keyboard::key::NUMPAD2;
        case XK_KP_3: return keyboard::key::NUMPAD3;
        case XK_KP_4: return keyboard::key::NUMPAD4;
        case XK_KP_5: return keyboard::key::NUMPAD5;
        case XK_KP_6: return keyboard::key::NUMPAD6;
        case XK_KP_7: return keyboard::key::NUMPAD7;
        case XK_KP_8: return keyboard::key::NUMPAD8;
        case XK_KP_9: return keyboard::key::NUMPAD9;
        case XK_F1: return keyboard::key::F1;
        case XK_F2: return keyboard::key::F2;
        case XK_F3: return keyboard::key::F3;
        case XK_F4: return keyboard::key::F4;
        case XK_F5: return keyboard::key::F5;
        case XK_F6: return keyboard::key::F6;
        case XK_F7: return keyboard::key::F7;
        case XK_F8: return keyboard::key::F8;
        case XK_F9: return keyboard::key::F9;
        case XK_F10: return keyboard::key::F10;
        case XK_F11: return keyboard::key::F11;
        case XK_F12: return keyboard::key::F12;
        case XK_Shift_L: return keyboard::key::L_SHIFT;
        case XK_Control_L: return keyboard::key::L_CTRL;
        case XK_Alt_L: return keyboard::key::L_ALT;
        case XK_Sys_Req: return keyboard::key::L_SYSTEM;
        case XK_Shift_R: return keyboard::key::R_SHIFT;
        case XK_Control_R: return keyboard::key::R_CTRL;
        case XK_Alt_R: return keyboard::key::R_ALT;
        //case XK_Sys_Req: return keyboard::key::R_SYSTEM;
        case XK_Tab: return keyboard::key::TAB;
        case XK_Caps_Lock: return keyboard::key::CAPS_LOCK;
        case XK_Escape: return keyboard::key::ESCAPE;
        case XK_Num_Lock: return keyboard::key::NUMLOCK;
        case XK_KP_Divide: return keyboard::key::DIVIDE;
        case XK_KP_Multiply: return keyboard::key::MULTIPLY;
        case XK_KP_Subtract: return keyboard::key::SUBTRACT;
        case XK_KP_Add: return keyboard::key::ADD;
        case XK_KP_Enter: return keyboard::key::ENTER;
        case XK_KP_Decimal: return keyboard::key::DECIMAL;
        case XK_Up: return keyboard::key::UP;
        case XK_Down: return keyboard::key::DOWN;
        case XK_Left: return keyboard::key::LEFT;
        case XK_Right: return keyboard::key::RIGHT;
        case XK_Insert: return keyboard::key::INSERT;
        case XK_Delete: return keyboard::key::DEL;
        case XK_Scroll_Lock: return keyboard::key::SCROLL_LOCK;
        case XK_Home: return keyboard::key::HOME;
        case XK_End: return keyboard::key::END;
        case XK_Pause: return keyboard::key::PAUSE;
        case XK_Page_Up: return keyboard::key::PAGE_UP;
        case XK_Page_Down: return keyboard::key::PAGE_DOWN;
        case XK_period: return keyboard::key::PERIOD;
        case XK_comma: return keyboard::key::COMMA;
        case XK_apostrophe: return keyboard::key::QUOTE;
        case XK_BackSpace: return keyboard::key::BACKSPACE;
        case XK_bracketleft: return keyboard::key::L_BRACKET;
        case XK_bracketright: return keyboard::key::R_BRACKET;
        case XK_slash: return keyboard::key::SLASH;
        case XK_backslash: return keyboard::key::BACKSLASH;
        case XK_semicolon: return keyboard::key::SEMICOLON;
        case XK_minus: return keyboard::key::HYPHEN;
        case XK_plus: return keyboard::key::EQUAL;
        case XK_Menu: return keyboard::key::MENU;
        case XK_grave: return keyboard::key::TILDE;
    }

    return keyboard::key::UNKNOWN;
}
#endif