#ifndef RENDERER_H
#define RENDERER_H

#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"

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

typedef struct Uniform
{
    float project[4 * 4];
    float view[4 * 4];
    float model[4 * 4];
} Uniform;



void Init(GLFWwindow *window);
void Quit();
void CreatePipeline(VkShaderModule vertexShader, VkShaderModule fragShader);
void CreateShaderModule(const char *fileName, VkShaderModule *shaderModule);
void Render();
void WaitIdle();

void createInstance(const char *extensions[], int *count);
void createSurface(GLFWwindow *window);
void pickupPhysicalDevice();
void queuePhysicalDevice();
void createDevice();
void createSwapchain();
void querySwapchainRequiredInfo();
void createImageView();
void createDescriptorSetLayout();
void createLayout(VkDescriptorSetLayout setLayout);
void createRenderPass();
void createFramebuffs();
void createCmdPool();
void createCmdBuff();
void recordCmd(VkCommandBuffer cmdBuff, VkFramebuffer fbo);
void createSemaphore();
void createFench();
void createVertices();
void createIndices();
void createUniform();
void createDescriptorPool();
void allocateDescriptorSet(VkDescriptorPool pool, VkDescriptorSetLayout setLayout);
void setVertexInputBindingDescription(VkVertexInputBindingDescription *bindingDes);
void setVertexInputAttrDescription(VkVertexInputAttributeDescription *attrDes);
void createBuffer(VkBufferUsageFlagBits flag, uint64_t size, VkBuffer *buf);
void queryMeminfo(VkBuffer buf, VkMemoryPropertyFlagBits flag);
void allocateMem(VkBuffer buf, VkMemoryPropertyFlagBits flag, VkDeviceMemory *mem);
int clamp(int value, int min, int max);
#endif
