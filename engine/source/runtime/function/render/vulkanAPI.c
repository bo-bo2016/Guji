#include "function/render/vulkanAPI.h"
#include "vulkan/vulkan.h"
#include <stdio.h>
#include <GLFW/glfw3.h>

typedef struct QueueFamilyIndices
{
	int graphicsFamily;
	int presentFamily;
} QueueFamilyIndices;
typedef struct SwapchainRequiredInfo{
	 VkSurfaceCapabilitiesKHR  capabilities;
	VkExtent2D extent;
	VkSurfaceFormatKHR format;
	VkPresentModeKHR presentMode;
	int imageCount;
} SwapchainRequiredInfo;

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
QueueFamilyIndices queueFamilyIndices;

int clamp(int value, int min, int max)
{
    if (value < min)
    {
        value = min;
    }
    else if (value > max)
    {
        value = max;
    }
    return value;
}
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
void pickPhysicalDevice(){
	unsigned int count;
	vkEnumeratePhysicalDevices(instance,&count,NULL);
	VkPhysicalDevice pDevices[count];
	vkEnumeratePhysicalDevices(instance,&count,pDevices);
	physicalDevice=pDevices[0];
}
void createLogicalDevice(){
	unsigned int queueFamilyCount=0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,&queueFamilyCount,NULL);	
	VkQueueFamilyProperties families[queueFamilyCount];
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,&queueFamilyCount,families);
	queueFamilyIndices.graphicsFamily=-1;
	queueFamilyIndices.presentFamily=-1;
	for(int i=0;i<queueFamilyCount;++i)
	{
		if(families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
			queueFamilyIndices.graphicsFamily=i;
		}
		VkBool32 presentSupport=VK_TRUE;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice,i,surface,&presentSupport);
		if(presentSupport){
			queueFamilyIndices.presentFamily=i;
		}
		if(queueFamilyIndices.graphicsFamily>=0&&queueFamilyIndices.presentFamily>=0){
			break;
		}
	}
	unsigned int count=queueFamilyIndices.graphicsFamily==queueFamilyIndices.presentFamily?1:2;
	VkDeviceQueueCreateInfo queueInfos[count];
	float priority=1.0;
	if(count==1){
		VkDeviceQueueCreateInfo info;
		info.sType=VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		info.queueFamilyIndex=queueFamilyIndices.graphicsFamily;
		info.queueCount=1;
		info.pQueuePriorities=&priority;
		info.flags=0;
		info.pNext=NULL;
		queueInfos[0]=info;
	}
	else{

		VkDeviceQueueCreateInfo info1;
		info1.sType=VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		info1.queueFamilyIndex=queueFamilyIndices.graphicsFamily;
		info1.queueCount=1;
		info1.pQueuePriorities=&priority;
		info1.flags=0;
		info1.pNext=NULL;
		queueInfos[0]=info1;

		VkDeviceQueueCreateInfo info2;
		info2.sType=VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		info2.queueFamilyIndex=queueFamilyIndices.presentFamily;
		info2.queueCount=1;
		info2.pQueuePriorities=&priority;
		info2.flags=0;
		info2.pNext=NULL;
		queueInfos[1]=info2;
	}
	VkDeviceCreateInfo info;
	info.sType=VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	info.queueCreateInfoCount=count;
	info.pQueueCreateInfos=queueInfos;
	info.pNext=NULL;
	info.pEnabledFeatures=NULL;
	info.enabledExtensionCount=1;
	info.ppEnabledExtensionNames=deviceExtensions;
	info.enabledLayerCount=0;
	info.ppEnabledLayerNames=NULL;
	info.flags=0;

	if(vkCreateDevice(physicalDevice,&info,NULL,&device)!=VK_SUCCESS){
		printf("failed to create logical device!\n");
	}
}
void createSwapChain(){
	return;
	SwapchainRequiredInfo requireInfo;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &requireInfo.capabilities);
	int formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, NULL);
	VkSurfaceFormatKHR formats[formatCount];
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats);
	requireInfo.format = formats[0];
	for (int i = 0; i < formatCount; ++i)
	{
		if (formats[i].format == VK_FORMAT_R8G8B8_SRGB ||
				formats[i].format == VK_FORMAT_B8G8R8_SRGB)
		{
			requireInfo.format = formats[i];
			break;
		}
	}
	requireInfo.extent.width = clamp(800,
			requireInfo.capabilities.minImageExtent.width,
			requireInfo.capabilities.maxImageExtent.width);
	requireInfo.extent.height = clamp(600,
			requireInfo.capabilities.minImageExtent.height,
			requireInfo.capabilities.maxImageExtent.height);
	requireInfo.imageCount = clamp(2,
			requireInfo.capabilities.minImageCount,
			requireInfo.capabilities.maxImageCount);
	int modeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &modeCount, NULL);
	VkPresentModeKHR modes[modeCount];
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &modeCount, modes);
	requireInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
	for (int i = 0; i < modeCount; ++i)
	{
		if (modes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			requireInfo.presentMode = modes[i];
			break;
		}
	}
	VkSwapchainCreateInfoKHR info;
	info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	info.pNext = NULL;
	info.flags = 0;
	info.surface = surface;
	info.minImageCount = requireInfo.imageCount;
	info.imageFormat = requireInfo.format.format;
	info.imageColorSpace = requireInfo.format.colorSpace;
	info.imageExtent = requireInfo.extent;
	info.imageArrayLayers = 1;
	info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	if (queueFamilyIndices.graphicsFamily= queueFamilyIndices.presentFamily)
	{
		int indices[] = {queueFamilyIndices.graphicsFamily};
		info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		info.queueFamilyIndexCount = 1;
		info.pQueueFamilyIndices = indices;
	}
	else
	{
		int indices[] = {queueFamilyIndices.graphicsFamily, queueFamilyIndices.presentFamily};
		info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		info.queueFamilyIndexCount = 2;
		info.pQueueFamilyIndices = indices;
	}
	info.preTransform = requireInfo.capabilities.currentTransform;
	info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	info.presentMode = requireInfo.presentMode;
	info.clipped = VK_TRUE;
	info.oldSwapchain = NULL;
	if(vkCreateSwapchainKHR(device, &info, NULL, &swapChain)!=VK_SUCCESS){
		printf("failed to create swap chain!");
	}
}
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


