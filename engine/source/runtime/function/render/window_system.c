#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <stdio.h>
#include "function/render/window_system.h"
#include <GLFW/glfw3.h>
#include "function/render/vulkanAPI.h"

void initialize()
{
	if(!glfwInit())
	{
		printf("fail to initialize GLFW");
		return;
	}
	glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
	GLFWwindow* window;
	window = glfwCreateWindow(800,600,"guji",NULL,NULL);
	while(!glfwWindowShouldClose(window)){
		glfwPollEvents();
		drawFrame();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

}
void mainLoop(){
}
void cleanup(){
}
