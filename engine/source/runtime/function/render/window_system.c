#include <stdio.h>
#include "runtime/function/render/window_system.h"

void initilize()
{
	if(!glfwInit())
	{
		printf("fail to initialize GLFW");
		return;
	}
	glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
	GLFWWindow* window;
	window = glfwCreateWindow(800,600,"guji",NULL,NULL);
	if(!window)
	{
		printf("failed to create window ");
		glfwTerminate();
		return;
	}
}
