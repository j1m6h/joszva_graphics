#include "../../inc/base/input_impl.h"
#include "../../inc/base/mouse.h"

namespace joszva::graphics
{
    namespace mouse
    {
        bool is_mouse_pressed(mouse::button btn)
        {
            return priv::input_impl::is_mouse_pressed(btn);
        }

        const vector2<int> get_cursor_coords()
        {
            return priv::input_impl::get_cursor_coords();
        }
    }
}