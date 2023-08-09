#include "Window.h"
#include <iostream>
#include "src2/Debugging/debugger.h"

Window::Window()
{
	glfwInit(); // Initialize glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set major and minor version so it matches version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Switch to Core profile instead of Immediate mode

	m_Window = glfwCreateWindow(1000, 1000, "Test Window", NULL, NULL); // Create a window
	if (m_Window == NULL) { // If failed, log and terminate GLFW
		std::cout << "Failed to create a window" << std::endl;
		glfwTerminate();
		glfwSetWindowShouldClose(m_Window, true);
		return;
	}

	glfwMakeContextCurrent(m_Window); // Make the context current window

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {	// Check if the GLAD/GLFW initialisation worked
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		glfwSetWindowShouldClose(m_Window, true);
	}

	GLCall(glViewport(0, 0, 1000, 1000));
}

Window::~Window()
{
}

GLFWwindow* Window::GetWindow()
{
    return nullptr;
}
