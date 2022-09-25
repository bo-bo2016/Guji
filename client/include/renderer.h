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

typedef struct SwapchainRequiredInfo
{
    VkSurfaceCapabilitiesKHR capabilities;
    VkExtent2D extent;
    VkSurfaceFormatKHR format;
    VkPresentModeKHR presentMode;
    int imageCount;
} SwapchainRequiredInfo;

typedef struct Vec2
{
    float x, y;
} Vec2;

typedef struct Color
{
    float r, g, b;
} Color;

typedef struct Vertex
{
    Vec2 position;
    Color color;
} Vertex;
typedef struct MemRequiredInfo
{
    int index;
    size_t size;
} MemRequiredInfo;

VkInstance instance;
VkSurfaceKHR surface;
VkPhysicalDevice phyDevice;
QueueFamilyIndices queueIndices;
VkDevice device;
VkQueue graphicsQueue;
VkQueue presentQueue;
VkSwapchainKHR swapchain;
SwapchainRequiredInfo requiredInfo;
int imageCount;
VkImage *pImage;
int imageViewCount;
VkImageView *pImageView;
VkPipeline pipeline;
VkShaderModule shaderModules[2];
VkPipelineLayout layout;
VkRenderPass renderPass;
VkFramebuffer framebuffs[2];
VkCommandPool cmdPool;
VkCommandBuffer cmdBuff;
VkSemaphore imageAvaliableSem;
VkSemaphore presentFinishSem;
VkFence fench;
VkBuffer vertexBuff;
VkDeviceMemory vertexMem;
MemRequiredInfo memReqInfo;
Vertex vertices[3];

void Init(SDL_Window *window);
void Quit();
void CreatePipeline(VkShaderModule vertexShader, VkShaderModule fragShader);
void CreateShaderModule(const char *fileName, VkShaderModule *shaderModule);
void Render();
void WaitIdle();

void createInstance(const char *extensions[], int *count);
void createSurface(SDL_Window *window);
void pickupPhysicalDevice();
void queuePhysicalDevice();
void createDevice();
void createSwapchain();
void querySwapchainRequiredInfo();
void createImageView();
void createLayout();
void createRenderPass();
void createFramebuffs();
void createCmdPool();
void createCmdBuff();
void recordCmd(VkCommandBuffer cmdBuff, VkFramebuffer fbo);
void createSemaphore();
void createFench();
void createVertices();
void setVertexInputBindingDescription(VkVertexInputBindingDescription *bindingDes);
void setVertexInputAttrDescription(VkVertexInputAttributeDescription *attrDes);
void createVertexBuff(VkBufferUsageFlagBits flag,VkBuffer *buf);
void queryMeminfo(VkBuffer buf,VkMemoryPropertyFlagBits flag);
void allocateMem(VkBuffer buf,VkDeviceMemory *mem);
int clamp(int value, int min, int max);
#endif