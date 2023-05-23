#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <stdio.h>
#include "function/render/window_system.h"
#include <GLFW/glfw3.h>
#include "function/render/renderer.h"

void run(){
	if(!glfwInit())
	{
		printf("fail to initialize GLFW");
		return;
	}
	glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
	GLFWwindow* window;
	window = glfwCreateWindow(800,600,"guji",NULL,NULL);
	Init(window);
	printf("init success!\n");
	printf("shader loaded success!\n");
	while(!glfwWindowShouldClose(window)){
		glfwPollEvents();
		Render();
	}
	WaitIdle();
	Quit();
	glfwDestroyWindow(window);
	glfwTerminate();
}
