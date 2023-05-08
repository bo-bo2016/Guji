#include "function/render/vulkanAPI.h"
#include "vulkan/vulkan.h"
#include <stdio.h>

const char* validationLayers[]={"VK_LAYER_KHRONOS_validation"};
const char* deviceExtensions[]={"VK_KHR_SWAPCHAIN_EXTENSION_NAME"};
//#ifdef NDEBUG
//const bool enableValidationLayers=false;
//#else
const int enableValidationLayers=1;
//#endif
VkInstance instance;
VkSurfaceKHR surface;
VkPhysicalDevice physicalDevice;
VkDevice device;
VkQueue graphicsQueue;
VkQueue presentQueue;
VkSwapchainKHR swapChain;

int checkValidationLayerSupport(){
		return 1;
}
void createInstance(){
		if(enableValidationLayers&&!checkValidationLayerSupport()){
				printf("validation layers requested, but not available!");
		}

}
void setupDebugMessenger(){}
void createSurface(){}
void pickPhysicalDevice(){}
void createLogicalDevice(){}
void createSwapChain(){}
void createImageViews(){}
void createRenderPass(){}
void createGraphicsPipeline(){}
void createFramebuffers(){}
void createCommandPool(){}
void createSyncObjects(){}
void cleanupSwapChain(){}

void initVulkan(){
		createInstance();
		setupDebugMessenger();
		createSurface();
		pickPhysicalDevice();
		createLogicalDevice();
		createSwapChain();
		createImageViews();
		createRenderPass();
		createGraphicsPipeline();
		createFramebuffers();
		createCommandPool();
		createSyncObjects();
}
void drawFrame(){

}
void cleanup(){

}


