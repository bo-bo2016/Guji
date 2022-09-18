#include "renderer.h"
#include <stdio.h>
#include <sys/stat.h>

#define CHECK_NULL(expr, name)         \
    if (!expr)                         \
    {                                  \
        printf("%s is NULL \n", name); \
    }
#define MAXSIZE 1024
void Init(SDL_Window *window)
{
    // get extensions
    unsigned int count;
    SDL_Vulkan_GetInstanceExtensions(window, &count, NULL);
    const char *extensions[count];
    SDL_Vulkan_GetInstanceExtensions(window, &count, extensions);
    for (int i = 0; i < count; ++i)
    {
        printf("%s \n", extensions[i]);
    }
    createInstance(extensions, &count);
    CHECK_NULL(instance, "instance");
    createSurface(window);
    CHECK_NULL(surface, "surface");
    pickupPhysicalDevice();
    CHECK_NULL(phyDevice, "phyDevice");
    queuePhysicalDevice();
    createDevice();
    CHECK_NULL(device, "device");
    vkGetDeviceQueue(device, queueIndices.graphicsIndices, 0, &graphicsQueue);
    vkGetDeviceQueue(device, queueIndices.presentIndices, 0, &presentQueue);
    CHECK_NULL(graphicsQueue, "graphicQueue");
    CHECK_NULL(presentQueue, "presentQueue");
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    querySwapchainRequiredInfo(w, h);
    createSwapchain();
    CHECK_NULL(swapchain, "swapchain");
    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, NULL);
    VkImage imgs[imageCount];
    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, imgs);
    pImage = imgs;
    createImageView();
    createLayout();
    CHECK_NULL(layout, "layout");
}
void Quit()
{
    vkDestroyPipelineLayout(device, layout, NULL);
    vkDestroyPipeline(device, pipeline, NULL);
    for (int i = 0; i < 2; ++i)
    {
        vkDestroyShaderModule(device, shaderModules[i], NULL);
    }
    for (int i = 0; i < imageViewCount; i++)
    {
        vkDestroyImageView(device, *(pImageView + i), NULL);
    }
    vkDestroyDevice(device, NULL);
    vkDestroySurfaceKHR(instance, surface, NULL);
    vkDestroyInstance(instance, NULL);
}
void CreatePipeline(VkShaderModule vertexShader, VkShaderModule fragShader)
{
    VkGraphicsPipelineCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    info.pNext = NULL;
    info.flags = 0;

    VkPipelineVertexInputStateCreateInfo vertexInputInfo;
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.pNext = NULL;
    vertexInputInfo.flags = 0;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = NULL;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = NULL;
    info.pVertexInputState = &vertexInputInfo;

    VkPipelineInputAssemblyStateCreateInfo inputAsmInfo;
    inputAsmInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAsmInfo.pNext = NULL;
    inputAsmInfo.flags = 0;
    inputAsmInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAsmInfo.primitiveRestartEnable = VK_FALSE;
    info.pInputAssemblyState = &inputAsmInfo;

    VkPipelineShaderStageCreateInfo stageInfos[2];
    stageInfos[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stageInfos[0].pNext = NULL;
    stageInfos[0].flags = 0;
    stageInfos[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    stageInfos[0].module = shaderModules[0];
    stageInfos[0].pName = "main";

    stageInfos[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stageInfos[1].pNext = NULL;
    stageInfos[1].flags = 0;
    stageInfos[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    stageInfos[1].module = shaderModules[1];
    stageInfos[1].pName = "main";
    info.stageCount = 2;
    info.pStages = stageInfos;

    VkPipelineViewportStateCreateInfo viewportState;
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.pNext = NULL;
    viewportState.flags = 0;
    viewportState.viewportCount = 1;
    VkViewport viewport;
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = requiredInfo.extent.width;
    viewport.height = requiredInfo.extent.height;
    viewport.minDepth = 0;
    viewport.maxDepth = 1;
    viewportState.pViewports = &viewport;
    VkRect2D scissor;
    VkOffset2D offset;
    offset.x = 0;
    offset.y = 0;
    scissor.offset = offset;
    scissor.extent = requiredInfo.extent;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;
    info.pViewportState = &viewportState;

    VkPipelineRasterizationStateCreateInfo rastInfo;
    rastInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rastInfo.pNext = NULL;
    rastInfo.flags = 0;
    rastInfo.depthClampEnable = VK_FALSE;
    rastInfo.rasterizerDiscardEnable = VK_FALSE;
    rastInfo.cullMode = VK_CULL_MODE_NONE;
    rastInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rastInfo.depthBiasEnable = VK_FALSE;
    rastInfo.lineWidth = 1;
    info.pRasterizationState = &rastInfo;

    VkPipelineMultisampleStateCreateInfo multisample;
    multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample.pNext = NULL;
    multisample.flags = 0;
    multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisample.sampleShadingEnable = VK_FALSE;
    info.pMultisampleState = &multisample;

    info.pDepthStencilState = NULL;

    VkPipelineColorBlendStateCreateInfo colorBlend;
    colorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlend.pNext = NULL;
    colorBlend.flags = 0;
    colorBlend.logicOpEnable = VK_FALSE;
    VkPipelineColorBlendAttachmentState attBlendState;
    attBlendState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlend.attachmentCount = 1;
    colorBlend.pAttachments = &attBlendState;
    info.pColorBlendState = &colorBlend;

    info.layout = layout;
    info.pDynamicState = NULL;
    info.pTessellationState = NULL;
    vkCreateGraphicsPipelines(device, NULL, 1, &info, NULL, &pipeline);
}
void CreateShaderModule(const char *filename, VkShaderModule *shaderModule)
{
    printf("%s\n", filename);
    FILE *in_file = fopen(filename, "rb");
    if (!in_file)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    struct stat sb;
    if (stat(filename, &sb) == -1)
    {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    char *file_contents = malloc(sb.st_size);
    fread(file_contents, sb.st_size, 1, in_file);
    printf("read data: %s\n", file_contents);
    fclose(in_file);

    VkShaderModuleCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.pNext = NULL;
    info.flags = 0;
    info.codeSize = sb.st_size;
    info.pCode = (const uint32_t *)file_contents;
    vkCreateShaderModule(device, &info, NULL, shaderModule);

    // free(file_contents);
}

void createInstance(const char *extensions[], int *count)
{
    // validation layers
    const char *const layers[] = {"VK_LAYER_KHRONOS_validation"};
    VkInstanceCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    info.pApplicationInfo = NULL;
    info.pNext = NULL;
    info.flags = 0;
    info.enabledExtensionCount = *count;
    info.ppEnabledExtensionNames = extensions;
    info.enabledLayerCount = 1;
    info.ppEnabledLayerNames = layers;
    vkCreateInstance(&info, NULL, &instance);
}
void createSurface(SDL_Window *window)
{
    SDL_Vulkan_CreateSurface(window, instance, &surface);
}
void pickupPhysicalDevice()
{
    unsigned int count;
    vkEnumeratePhysicalDevices(instance, &count, NULL);
    VkPhysicalDevice pDevices[count];
    vkEnumeratePhysicalDevices(instance, &count, pDevices);
    phyDevice = pDevices[0];
    // VkPhysicalDeviceProperties *pros;
    // vkGetPhysicalDeviceProperties(phyDevice, pros);
    // printf("deviceName=%s", pros->deviceName);
}
void queuePhysicalDevice()
{
    unsigned int count;
    vkGetPhysicalDeviceQueueFamilyProperties(phyDevice, &count, NULL);
    VkQueueFamilyProperties families[count];
    vkGetPhysicalDeviceQueueFamilyProperties(phyDevice, &count, families);
    unsigned int idx = 0;
    queueIndices.graphicsIndices = queueIndices.presentIndices = -1;
    for (int i = 0; i < count; ++i)
    {
        if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            queueIndices.graphicsIndices = idx;
        }
        VkBool32 supported = VK_TRUE;
        vkGetPhysicalDeviceSurfaceSupportKHR(phyDevice, idx, surface, &supported);
        if (supported)
        {
            queueIndices.presentIndices = idx;
        }
        if (queueIndices.graphicsIndices >= 0 && queueIndices.presentIndices >= 0)
        {
            break;
        }
        idx++;
    }
}
void createDevice()
{
    unsigned int count = queueIndices.graphicsIndices == queueIndices.presentIndices ? 1 : 2;
    VkDeviceQueueCreateInfo queueInfos[count];
    if (count == 1)
    {
        VkDeviceQueueCreateInfo info;
        info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        float piority = 1.0;
        info.queueFamilyIndex = queueIndices.graphicsIndices;
        info.queueCount = 1;
        info.pQueuePriorities = &piority;
        info.flags = 0;
        info.pNext = NULL;
        queueInfos[0] = info;
    }
    else
    {
        VkDeviceQueueCreateInfo info1;
        float piority = 1.0;
        info1.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        info1.queueFamilyIndex = queueIndices.graphicsIndices;
        info1.queueCount = 1;
        info1.pQueuePriorities = &piority;
        info1.flags = 0;
        info1.pNext = NULL;
        queueInfos[0] = info1;

        VkDeviceQueueCreateInfo info2;
        info2.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        info2.queueFamilyIndex = queueIndices.presentIndices;
        info2.queueCount = 1;
        info2.pQueuePriorities = &piority;
        info2.flags = 0;
        info2.pNext = NULL;
        queueInfos[1] = info2;
    }

    VkDeviceCreateInfo info;
    const char *extensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    info.queueCreateInfoCount = count;
    info.pQueueCreateInfos = queueInfos;
    info.pNext = NULL;
    info.pEnabledFeatures = NULL;
    info.enabledExtensionCount = 1;
    info.ppEnabledExtensionNames = extensions;
    info.enabledLayerCount = 0;
    info.ppEnabledLayerNames = NULL;
    info.flags = 0;

    vkCreateDevice(phyDevice, &info, NULL, &device);
}
void createSwapchain()
{
    VkSwapchainCreateInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    info.pNext = NULL;
    info.flags = 0;
    info.surface = surface;
    info.minImageCount = requiredInfo.imageCount;
    info.imageFormat = requiredInfo.format.format;
    info.imageColorSpace = requiredInfo.format.colorSpace;
    info.imageExtent = requiredInfo.extent;
    info.imageArrayLayers = 1;
    info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    if (queueIndices.graphicsIndices == queueIndices.presentIndices)
    {
        int indices[] = {queueIndices.graphicsIndices};
        info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        info.queueFamilyIndexCount = 1;
        info.pQueueFamilyIndices = indices;
    }
    else
    {
        int indices[] = {queueIndices.graphicsIndices, queueIndices.presentIndices};
        info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        info.queueFamilyIndexCount = 2;
        info.pQueueFamilyIndices = indices;
    }
    info.preTransform = requiredInfo.capabilities.currentTransform;
    info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    info.presentMode = requiredInfo.presentMode;
    info.clipped = VK_TRUE;
    info.oldSwapchain = NULL;
    vkCreateSwapchainKHR(device, &info, NULL, &swapchain);
}
void querySwapchainRequiredInfo(int w, int h)
{
    SwapchainRequiredInfo info;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(phyDevice, surface, &info.capabilities);
    int formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(phyDevice, surface, &formatCount, NULL);
    VkSurfaceFormatKHR formats[formatCount];
    vkGetPhysicalDeviceSurfaceFormatsKHR(phyDevice, surface, &formatCount, formats);
    info.format = formats[0];
    for (int i = 0; i < formatCount; ++i)
    {
        if (formats[i].format == VK_FORMAT_R8G8B8_SRGB ||
            formats[i].format == VK_FORMAT_B8G8R8_SRGB)
        {
            info.format = formats[i];
            break;
        }
    }
    info.extent.width = clamp(w,
                              info.capabilities.minImageExtent.width,
                              info.capabilities.maxImageExtent.width);
    info.extent.height = clamp(h,
                               info.capabilities.minImageExtent.height,
                               info.capabilities.maxImageExtent.height);
    info.imageCount = clamp(2,
                            info.capabilities.minImageCount,
                            info.capabilities.maxImageCount);
    int modeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(phyDevice, surface, &modeCount, NULL);
    VkPresentModeKHR modes[modeCount];
    vkGetPhysicalDeviceSurfacePresentModesKHR(phyDevice, surface, &modeCount, modes);
    info.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (int i = 0; i < modeCount; ++i)
    {
        if (modes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            info.presentMode = modes[i];
            break;
        }
    }
    requiredInfo = info;
}
void createImageView()
{
    imageViewCount = imageCount;
    pImageView = malloc(sizeof(VkImageView) * imageViewCount);
    for (int i = 0; i < imageViewCount; ++i)
    {
        VkImageViewCreateInfo info;
        info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        info.pNext = NULL;
        info.flags = 0;
        info.image = pImage[i];
        info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        info.format = requiredInfo.format.format;
        VkComponentMapping mapping;
        mapping.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        mapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        mapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        mapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        info.components = mapping;
        VkImageSubresourceRange range;
        range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        range.baseMipLevel = 0;
        range.levelCount = 1;
        range.baseArrayLayer = 0;
        range.layerCount = 1;
        info.subresourceRange = range;
        vkCreateImageView(device, &info, NULL, pImageView + i);
    }
}
void createLayout()
{
    VkPipelineLayoutCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    info.pNext = NULL;
    info.flags = 0;
    info.setLayoutCount = 0;
    info.pSetLayouts = NULL;
    info.pushConstantRangeCount = 0;
    info.pPushConstantRanges = NULL;
    vkCreatePipelineLayout(device, &info, NULL, &layout);
}
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
