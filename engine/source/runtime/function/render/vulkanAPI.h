#ifndef VULKANAPT_H
#define VULKANAPI_H

#include "vulkan/vulkan.h"

const char* validationLayers[]={"VK_LAYER_KHRONOS_validation"};
const char* deviceExtensions[]={"VK_KHR_SWAPCHAIN_EXTENSION_NAME"};
VkInstance instance;
VkSurfaceKHR surface;
VkPhysicalDevice physicalDevice;
VkDevice device;
VkQueue graphicsQueue;
VkQueue presentQueue;
VkSwapchainKHR swapChain;


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
