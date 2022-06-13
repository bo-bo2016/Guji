#ifndef RENDERER_H
#define RENDERER_H

#include "vulkan/vulkan.h"
#include "SDL.h"
#include "SDL_vulkan.h"

VkInstance instance;
VkSurfaceKHR surface;
void Init(SDL_Window* window);
void Quit();

#endif