#include "../../inc/base/input_impl.h"
#include "../../inc/base/mouse.h"

namespace joszva::engine
{
    namespace mouse
    {
        bool is_mouse_pressed(mouse::button btn)
        {
            return priv::input_impl::is_mouse_pressed(btn);
        }
    }
}