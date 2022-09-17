#ifndef RENDERER_H
#define RENDERER_H

#include "vulkan/vulkan.h"
#include "SDL.h"
#include "SDL_vulkan.h"

typedef struct QueueFamilyIndices
{
    int graphicsIndices;
    int presentIndices;
} QueueFamilyIndices;

VkInstance instance;
VkSurfaceKHR surface;
VkPhysicalDevice phyDevice;
int queueFamilyIndex;
VkDevice device;
VkQueue graphicsQueue;
VkQueue presentQueue;
void Init(SDL_Window *window);
void Quit();
void createInstance(const char *extensions[], int *count);
void createSurface(SDL_Window *window);
void pickupPhysicalDevice();
void queuePhysicalDevice();
void createDevice();
#endif