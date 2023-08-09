#include "../Window.h"
#include "Debugging/debugger.h"
#include "Utils/Time/Time.h"

using namespace LearnOGL;

	//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	//void processInput(GLFWwindow* window);
	//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	Window window;

	int main() {
		Time::Init();
		

		GLCall(glViewport(0, 0, 1000, 1000));
		//GLCall(glfwSetFramebufferSizeCallback(window.GetWindow(), framebuffer_size_callback));
		//GLCall(glfwSetCursorPosCallback(window.GetWindow(), mouse_callback));
		//GLCall(glfwSetScrollCallback(window.GetWindow(), scroll_callback));



		while (!glfwWindowShouldClose(window.GetWindow())) {
			GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		}

		return 0;
	}
