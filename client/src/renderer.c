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
    // CHECK_NULL(device, "device");
    // vkGetDeviceQueue(device, queueIndices.graphicsIndices, 0, &graphicsQueue);
    // vkGetDeviceQueue(device, queueIndices.presentIndices, 0, &presentQueue);
    // CHECK_NULL(graphicsQueue, "graphicQueue");
    // CHECK_NULL(presentQueue, "presentQueue");
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
    // VkPhysicalDeviceProperties *pros;
    // vkGetPhysicalDeviceProperties(phyDevice, pros);
    // printf("deviceName=%s", pros->deviceName);
}
void queuePhysicalDevice()
{
    unsigned int count;
    vkGetPhysicalDeviceQueueFamilyProperties(phyDevice, &count, NULL);
    VkQueueFamilyProperties families[count];
    vkGetPhysicalDeviceQueueFamilyProperties(phyDevice, &count, families);
    unsigned int idx = 0;
    queueIndices.graphicsIndices = queueIndices.presentIndices = -1;
    for (int i = 0; i < count; ++i)
    {
        if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            queueIndices.graphicsIndices = idx;
        }
        VkBool32 supported = VK_TRUE;
        vkGetPhysicalDeviceSurfaceSupportKHR(phyDevice, idx, surface, &supported);
        if (supported)
        {
            queueIndices.presentIndices = idx;
        }
        if (queueIndices.graphicsIndices >= 0 && queueIndices.presentIndices >= 0)
        {
            break;
        }
        idx++;
    }
}
void createDevice()
{
    unsigned int count = queueIndices.graphicsIndices == queueIndices.presentIndices ? 1 : 2;
    VkDeviceQueueCreateInfo queueInfos[count];
    if (count == 1)
    {
        VkDeviceQueueCreateInfo info;
        info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        float piority = 1.0;
        info.queueFamilyIndex = queueIndices.graphicsIndices;
        info.queueCount = 1;
        info.pQueuePriorities = &piority;
        info.flags = 0;
        info.pNext = NULL;
        queueInfos[0] = info;
    }
    else
    {
        VkDeviceQueueCreateInfo info1;
        float piority = 1.0;
        info1.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        info1.queueFamilyIndex = queueIndices.graphicsIndices;
        info1.queueCount = 1;
        info1.pQueuePriorities = &piority;
        info1.flags = 0;
        info1.pNext = NULL;
        queueInfos[0] = info1;

        VkDeviceQueueCreateInfo info2;
        info2.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        info2.queueFamilyIndex = queueIndices.presentIndices;
        info2.queueCount = 1;
        info2.pQueuePriorities = &piority;
        info2.flags = 0;
        info2.pNext = NULL;
        queueInfos[1] = info2;
    }

    VkDeviceCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    info.queueCreateInfoCount = count;
    info.pQueueCreateInfos = queueInfos;
    info.pNext = NULL;
    info.pEnabledFeatures = NULL;
    info.enabledExtensionCount = 0;
    info.ppEnabledExtensionNames = NULL;
    info.enabledLayerCount = 0;
    info.ppEnabledLayerNames = NULL;
    info.flags = 0;

    vkCreateDevice(phyDevice, &info, NULL, &device);
}
