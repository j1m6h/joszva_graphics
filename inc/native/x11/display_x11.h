#ifndef DISPLAY_X11_H
#define DISPLAY_X11_H

#include "../../base/config.h"

#ifdef JOSZVA_SYS_LINUX
namespace joszva::engine::priv
{
    namespace display_x11
    {
        Display* open_display();
        void close_display(Display* display);
    }
}
#endif
#endif