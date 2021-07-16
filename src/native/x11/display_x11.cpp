#include <mutex>

#include "../../../inc/native/x11/display_x11.h"

namespace joszva::engine::priv
{
    namespace display_x11
    {
        Display* shared_display = nullptr;
        unsigned int display_counter = 0;

        std::mutex display_mutex;

        Display* open_display()
        {
            std::lock_guard<std::mutex> lock(display_mutex);

            if (display_counter == 0)
            {
                shared_display = XOpenDisplay(static_cast<char*>(0));
            }

            ++display_counter;
            return shared_display;
        }

        void close_display(Display* display)
        {
            --display_counter;
            if (display_counter == 0)
            {
                XCloseDisplay(shared_display);
            }
        }
    }
}