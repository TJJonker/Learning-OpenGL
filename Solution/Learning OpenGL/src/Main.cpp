#include <glad/glad.h>	// Include this file first
#include <GLFW/glfw3.h>	// Then this one
#include <iostream>
#include "Debugging/debugger.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
	glfwInit(); // Initialize glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set major and minor version so it matches version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Switch to Core profile instead of Immediate mode

	GLFWwindow* window = glfwCreateWindow(1600, 900, "Test Window", NULL, NULL); // Create a window
	if (window == NULL) { // If failed, log and terminate GLFW
		std::cout << "Failed to create a window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); // Make the context current window

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {	// Check if the GLAD/GLFW initialisation worked
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	GLCall(glViewport(0, 0, 1600, 900));
	GLCall(glfwSetFramebufferSizeCallback(window, framebuffer_size_callback));


	// Render loop
	while (!glfwWindowShouldClose(window)) {
		// Inputs
		processInput(window);

		// Render instructions
		GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		// Check and call events and swap the buffers
		GLCall(glfwSwapBuffers(window));
		GLCall(glfwPollEvents());
	}

	glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	GLCall(glViewport(0, 0, width, height));
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		GLCall(glfwSetWindowShouldClose(window, true));
	}
}
