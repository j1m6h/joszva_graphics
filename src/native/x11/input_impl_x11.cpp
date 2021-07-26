#include "../../../inc/native/x11/display_x11.h"
#include "../../../inc/native/x11/input_impl_x11.h"

#ifdef JOSZVA_SYS_LINUX
using joszva::graphics::vector2;

namespace joszva::graphics::priv
{
    namespace input_impl_x11
    {
        bool is_key_pressed(keyboard::key _key)
        {
            KeySym keysym = 0;
            switch (_key)
            {
                case keyboard::key::A: keysym = XK_A; break;
                case keyboard::key::B: keysym = XK_B; break;
            }

            Display* display = display_x11::open_display();

            KeyCode code = XKeysymToKeycode(display, keysym);
            if (code != 0)
            {
                char keys[32];
                XQueryKeymap(display, keys);

                display_x11::close_display(display);

                return (keys[code / 8] & (1 << (code % 8))) != 0;
            }

            display_x11::close_display(display);

            return false;
        }

        bool is_mouse_pressed(mouse::button btn)
        {
            Display* display = display_x11::open_display();

            Window root, child;
            int root_x, root_y;
            int win_x, win_y;
            unsigned int btns;

            XQueryPointer(display, DefaultRootWindow(display), &root, &child, &win_x, &win_y, &root_x, &root_y, &btns);
            display_x11::close_display(display);

            switch (btn)
            {
                case mouse::button::LEFT: return btns & Button1Mask;
                case mouse::button::MIDDLE: return btns & Button2Mask;
                case mouse::button::RIGHT: return btns & Button3Mask;
                case mouse::button::EXTRA1: return false;
                case mouse::button::EXTRA2: return false;
                default: return false;
            }

            return false;
        }

        const vector2<int> get_cursor_coords()
        {
            Display* display = display_x11::open_display();

            /* we only care about root_x, and root_y in this case */
            Window root, child;
            int root_x, root_y;
            int win_x, win_y;
            unsigned int btns;

            XQueryPointer(display, DefaultRootWindow(display), &root, &child, &win_x, &win_y, &root_x, &root_y, &btns);
            display_x11::close_display(display);

            return vector2<int>(root_x, root_y);
        }

        const vector2<int> get_cursor_coords(const window_impl_x11& win)
        {
            if (win.get_handle())
            {
                Display* display = display_x11::open_display();

                /* we only care about win_x, and win_y in this case */
                Window root, child;
                int root_x, root_y;
                int win_x, win_y;
                unsigned int btns;

                XQueryPointer(display, win.get_handle(), &root, &child, &root_x, &root_y, &win_x, &win_y, &btns);
                display_x11::close_display(display);

                return vector2<int>(win_x, win_y);
            }

            return vector2<int>(0, 0);
        }

        void set_cursor_coords(const window_impl_x11& win, const vector2<int>& point)
        {     
        }
    }
}
#endif