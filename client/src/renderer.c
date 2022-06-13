#include "renderer.h"
#include <stdio.h>

void Init(SDL_Window *window)
{
    unsigned int count;
    SDL_Vulkan_GetInstanceExtensions(window, &count, NULL);
    const char *extensions[count];
    SDL_Vulkan_GetInstanceExtensions(window, &count, extensions);
    for (int i = 0; i < count; ++i)
    {
        printf("%s \n", extensions[i]);
    }
    VkInstanceCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    info.ppEnabledExtensionNames = extensions;
    vkCreateInstance(&info, NULL, &instance);
    if (!instance)
    {
        printf("instance create fail!");
    }
}
void Quit()
{
    vkDestroyInstance(instance, NULL);
}