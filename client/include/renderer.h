#ifndef RENDERER_H
#define RENDERER_H

#include "vulkan/vulkan.h"
#include "SDL.h"
#include "SDL_vulkan.h"

VkInstance instance;
VkSurfaceKHR surface;
VkPhysicalDevice phyDevice;
void Init(SDL_Window *window);
void createInstance(const char *extensions[], int *count);
void createSurface(SDL_Window *window);
void pickupPhysicalDevice();
void Quit();

#endif