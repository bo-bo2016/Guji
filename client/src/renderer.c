#include "renderer.h"
#include <stdio.h>

instance = NULL;
void Init()
{
    VkInstanceCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    vkCreateInstance(&info, NULL, instance);
    if (!instance)
    {
        printf("instance create fail!");
    }
}
void Quit()
{
    instance.destroy();
}