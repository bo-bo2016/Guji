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
    queuePhysicalDevice();
    createDevice();
    CHECK_NULL(device, "device");
    vkGetDeviceQueue(device, queueFamilyIndex, 0, &graphicsQueue);
    vkGetDeviceQueue(device, queueFamilyIndex, 0, &presentQueue);
    CHECK_NULL(graphicsQueue, "graphicQueue");
    CHECK_NULL(presentQueue, "presentQueue");
}
void Quit()
{
    vkDestroyDevice(device, NULL);
    vkDestroySurfaceKHR(instance, surface, NULL);
    vkDestroyInstance(instance, NULL);
}
void createInstance(const char *extensions[], int *count)
{
    // validation layers
    const char *const layers[] = {"VK_LAYER_KHRONOS_validation"};
    VkInstanceCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    info.pApplicationInfo = NULL;
    info.pNext = NULL;
    info.flags = 0;
    info.enabledExtensionCount = *count;
    info.ppEnabledExtensionNames = extensions;
    info.enabledLayerCount = 1;
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
}
void queuePhysicalDevice()
{
    unsigned int count;
    vkGetPhysicalDeviceQueueFamilyProperties(phyDevice, &count, NULL);
    VkQueueFamilyProperties families[count];
    vkGetPhysicalDeviceQueueFamilyProperties(phyDevice, &count, families);
    unsigned int idx = 0;
    for (int i = 0; i < count; ++i)
    {
        if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            queueFamilyIndex = idx;
            break;
        }
        idx++;
    }
}
void createDevice()
{
    VkDeviceQueueCreateInfo queueInfo = {};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = queueFamilyIndex;
    queueInfo.queueCount = 1;
    float priority[] = {1.0};
    queueInfo.pQueuePriorities = priority;
    queueInfo.pNext = NULL;

    VkDeviceCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    info.queueCreateInfoCount = 1;
    info.pQueueCreateInfos = &queueInfo;
    info.pNext = NULL;
    info.pEnabledFeatures = NULL;
    info.enabledExtensionCount = 0;
    info.ppEnabledExtensionNames = NULL;
    info.enabledLayerCount = 0;
    info.ppEnabledLayerNames = NULL;
    info.flags = 0;
    
    vkCreateDevice(phyDevice, &info, NULL, &device);
}
