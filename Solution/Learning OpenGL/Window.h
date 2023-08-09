#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Window
{

private:
	GLFWwindow* m_Window;

public:
	Window();
	~Window();
	GLFWwindow* GetWindow();
};

