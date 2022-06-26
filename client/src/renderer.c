#include "renderer.h"
#include <stdio.h>

#define CHECK_NULL(expr, name)         \
    if (!expr)                         \
    {                                  \
        printf("%s is NULL \n", name); \
    }
void Init(SDL_Window *window)
{
    // get extensions
    unsigned int count;
    SDL_Vulkan_GetInstanceExtensions(window, &count, NULL);
    const char *extensions[count];
    SDL_Vulkan_GetInstanceExtensions(window, &count, extensions);
    for (int i = 0; i < count; ++i)
    {
        printf("%s \n", extensions[i]);
    }
    createInstance(extensions, &count);
    CHECK_NULL(instance, "instance");
    createSurface(window);
    CHECK_NULL(surface, "surface");
    pickupPhysicalDevice();
    CHECK_NULL(phyDevice, "phyDevice");
}
void createInstance(const char *extensions[], int *count)
{
    // validation layers
    const char *const layers[] = {"VK_LAYER_KHRONOS_validation"};
    VkInstanceCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    info.pApplicationInfo = NULL;
    info.enabledExtensionCount = *count;
    info.enabledLayerCount = 0;
    info.ppEnabledExtensionNames = extensions;
    info.pNext = NULL;
    info.ppEnabledLayerNames = layers;
    vkCreateInstance(&info, NULL, &instance);
}
void createSurface(SDL_Window *window)
{
    SDL_Vulkan_CreateSurface(window, instance, &surface);
}
void pickupPhysicalDevice()
{
    unsigned int count;
    vkEnumeratePhysicalDevices(instance, &count, NULL);
    VkPhysicalDevice pDevices[count];
    vkEnumeratePhysicalDevices(instance, &count, pDevices);
    phyDevice = pDevices[0];
    // VkPhysicalDeviceProperties *pros;
    // vkGetPhysicalDeviceProperties(phyDevice, pros);
    // printf("deviceName=%s", pros->deviceName);
}
void Quit()
{
    vkDestroySurfaceKHR(instance, surface, NULL);
    vkDestroyInstance(instance, NULL);
}