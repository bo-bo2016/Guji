#include "function/render/vulkanAPI.h"
#include "vulkan/vulkan.h"
#include <stdio.h>
#include <GLFW/glfw3.h>

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

		uint32_t count;
		glfwGetRequiredInstanceExtensions(&count);
		const char** extensions=glfwGetRequiredInstanceExtensions(&count);
		for (int i = 0; i < count; ++i)
		{
				printf("%s \n", extensions[i]);
		}

		VkInstanceCreateInfo info;
		info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		info.pApplicationInfo = NULL;
		info.pNext = NULL;
		info.flags = 0;
		info.enabledExtensionCount = count;
		info.ppEnabledExtensionNames = extensions;
		info.enabledLayerCount = 1;
		info.ppEnabledLayerNames = validationLayers;
		vkCreateInstance(&info, NULL, &instance);
}
void setupDebugMessenger(){}
void createSurface(GLFWwindow *window){
		if(glfwCreateWindowSurface(instance,window,NULL,&surface)!=VK_SUCCESS){
				printf("failed to create window surface!");
		}	
}
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

void initVulkan (GLFWwindow*  window){
		createInstance();
		setupDebugMessenger();
		createSurface(window);
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


