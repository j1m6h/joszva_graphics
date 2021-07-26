#include "../../inc/base/input_impl.h"
#include "../../inc/base/keyboard.h"

namespace joszva::graphics
{
    namespace keyboard
    {
        bool is_key_pressed(key k)
        {
            return priv::input_impl::is_key_pressed(k);
        }
    }
}