#ifndef VULKANAPT_H
#define VULKANAPI_H

#include "vulkan/vulkan.h"

void initVulkan();
void createInstance();
void setupDebugMessager();
void createsurface();
void pickPhysicalDevice();
void createLogicDevice();
void createSwapChain();
void createImageViews();
void createRenderPass();
void createGraphicsPipeline();
void createFramebuffers();
void createCommandPools();
void createSyncObjects();
void cleanupSwapChain();
void drawFrame();

#endif /*VULKANAPI_H*/
