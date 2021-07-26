#include <cstring>
#include <vector>

#include "../../../inc/native/win32/vulkan_impl_win32.h" /* must be included before vulkan_win32.h */
#include <vulkan/vulkan_win32.h>

namespace joszva::graphics::priv
{
	namespace vulkan_impl_win32
	{
		char* enabled_exts[2];

		const char** get_required_instance_extensions(uint32_t* count)
		{
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
				else if (std::strcmp(it->extensionName, VK_KHR_WIN32_SURFACE_EXTENSION_NAME) == 0)
				{
					enabled_exts[1] = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
					*count = 2;
				}
			}

			return const_cast<const char**>(enabled_exts);
		}

		VkResult create_vulkan_surface(const window_impl_win32& win, const VkInstance& instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR& surface)
		{
			VkWin32SurfaceCreateInfoKHR create_info{};
			create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			create_info.hwnd = win.get_handle();
			create_info.hinstance = GetModuleHandle(nullptr);

			VkResult result = vkCreateWin32SurfaceKHR(instance, &create_info, allocator, &surface);
			return result;
		}
	}
}