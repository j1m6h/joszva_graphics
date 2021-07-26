#include <cstring>
#include <vector>

#include "../../../inc/native/x11/display_x11.h"
#include "../../../inc/native/x11/vulkan_impl_x11.h"

namespace joszva::graphics::priv
{
    namespace vulkan_impl_x11
    {
        char* enabled_exts[2];

        const char **get_required_instance_extensions(uint32_t *count)
        {
            /* in case it wasn't 0 already */
            *count = 0;

            vkEnumerateInstanceExtensionProperties(nullptr, count, nullptr);
            
            std::vector<VkExtensionProperties> extensions(*count);
            vkEnumerateInstanceExtensionProperties(nullptr, count, extensions.data());

            for (auto it = extensions.begin(); it != extensions.end(); ++it)
            {
                if (std::strcmp(it->extensionName, VK_KHR_SURFACE_EXTENSION_NAME) == 0)
                {
                    enabled_exts[0] = VK_KHR_SURFACE_EXTENSION_NAME;
                    *count = 1;
                }
                else if (strcmp(it->extensionName, VK_KHR_XLIB_SURFACE_EXTENSION_NAME) == 0)
                {
                    enabled_exts[1] = VK_KHR_XLIB_SURFACE_EXTENSION_NAME;
                    *count = 2;
                }
            }

            return const_cast<const char **>(enabled_exts);
        }

        VkResult create_vulkan_surface(const window_impl_x11& win, const VkInstance& instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR& surface)
        {
            Display* display = display_x11::open_display();

            VkXlibSurfaceCreateInfoKHR create_info{};
            create_info.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
            create_info.dpy = display;
            create_info.window = win.get_handle();

            VkResult result = vkCreateXlibSurfaceKHR(instance, &create_info, allocator, &surface);

            display_x11::close_display(display);

            return result;
        }
    }
}