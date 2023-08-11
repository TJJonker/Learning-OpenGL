#include <glad/glad.h>	// Include this file first
#include <GLFW/glfw3.h>	// Then this one
#include <iostream>
#include "Debugging/debugger.h"
#include "vendor/stb_lib/stb_lib.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Utils/Time.h"
#include "OpenGL Core/VertexArray/VertexArray.h"
#include "OpenGL Core/IndexBuffer/IndexBuffer.h"
#include "OpenGL Core/Shader/Shader.h"
#include "OpenGL Core/Renderer/Renderer.h"
#include "Camera/Camera.h"
#include "OpenGL Core/Textures/Texture.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Camera camera;
Renderer renderer;

int main() {
	glfwInit(); // Initialize glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set major and minor version so it matches version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Switch to Core profile instead of Immediate mode

	GLFWwindow* window = glfwCreateWindow(1000, 1000, "Test Window", NULL, NULL); // Create a window
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

	GLCall(glViewport(0, 0, 1000, 1000));
	GLCall(glfwSetFramebufferSizeCallback(window, framebuffer_size_callback));
	GLCall(glfwSetCursorPosCallback(window, mouse_callback));
	GLCall(glfwSetScrollCallback(window, scroll_callback));

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	{
		float vertices[] = {
			// positions			// normals				// texture coords
			-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
			
			-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f, 1.0f,
			
			-0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
			
			 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
			
			 0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,
			
			 0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f


		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			8, 9, 10,
			10, 11, 8,

			12, 13, 14,
			14, 15, 12,

			16, 17, 18,
			18, 19, 16,

			20, 21, 22,
			22, 23, 20
		};


		///////// Lit object

		VertexArray vertexArray;
		int verticesArrayLength = sizeof(vertices) / sizeof(float);
		VertexBuffer vertexBuffer(vertices, verticesArrayLength * sizeof(float));
		VertexBufferLayout vertexBufferLayout;

		vertexBufferLayout.Push<float>(3);
		vertexBufferLayout.Push<float>(3);
		vertexBufferLayout.Push<float>(2);
		vertexArray.AddBuffer(vertexBuffer, vertexBufferLayout);

		IndexBuffer indexBuffer(indices, 36);

		Texture diffuseMap("src/textures/container.png");
		Texture specularMap("src/textures/container_specular.png");
		glActiveTexture(GL_TEXTURE0);
		diffuseMap.Bind();
		glActiveTexture(GL_TEXTURE1);
		specularMap.Bind();

		Shader shaderLit("src/shaders/vertexShaderLighting.glsl", "src/shaders/fragmentShaderLighting.glsl");


		//////// Light source

		VertexArray lightVertexArray;
		lightVertexArray.AddBuffer(vertexBuffer, vertexBufferLayout);
		Shader lightSourceShader("src/shaders/vertexShaderLight.glsl", "src/shaders/fragmentShaderLight.glsl");
		glm::vec3 lightPos(0.0f, 0.5f, 2.0f);


		GLCall(glEnable(GL_DEPTH_TEST));
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// Render loop
		while (!glfwWindowShouldClose(window)) {
			// Inputs
			processInput(window);

			Time::Update();

			// Render instructions
			GLCall(glClearColor(0.1f, 0.1f, 0.125f, 1.0f));
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

			glm::vec3 lightColor;
			lightColor.x = sin(glfwGetTime() * 2.0f);
			lightColor.y = sin(glfwGetTime() * 0.7f);
			lightColor.z = sin(glfwGetTime() * 1.3f);

			glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
			glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

			shaderLit.Bind();

			shaderLit.Set3f("lightPosition", lightPos);
			shaderLit.Set3f("lightColor", 1.0f, 1.0f, 1.0f);
			shaderLit.Set3f("material.ambient", 1.0f, 0.5f, 0.31f);
			shaderLit.SetInt("material.diffuse", 0);
			shaderLit.SetInt("material.specular", 1);
			shaderLit.SetFloat("material.shininess", 32.0f);
			shaderLit.Set3f("light.ambient", 0.2f, 0.2f, 0.2f);
			shaderLit.Set3f("light.diffuse", 0.5f, 0.5f, 0.5f); 
			shaderLit.Set3f("light.specular", 1.0f, 1.0f, 1.0f);
			shaderLit.Set3f("light.position", lightPos);
			shaderLit.Set3f("viewPosition", camera.GetPosition());

			glm::mat4 projection = glm::perspective(glm::radians(camera.GetFOV()), 1000.0f / 1000.0f, 0.1f, 100.0f);
			glm::mat4 viewMatrix = camera.GetView();
			shaderLit.SetMatrix4("projection", projection);
			shaderLit.SetMatrix4("view", viewMatrix);

			glm::mat4 model = glm::mat4(1.0f);
			shaderLit.SetMatrix4("model", model);

			renderer.Draw(vertexArray, indexBuffer, shaderLit);



			lightSourceShader.Bind();
			lightSourceShader.SetMatrix4("projection", projection);
			lightSourceShader.SetMatrix4("view", viewMatrix);
			model = glm::mat4(1.0f);
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.2f));
			lightSourceShader.SetMatrix4("model", model);
			renderer.Draw(lightVertexArray, indexBuffer, lightSourceShader);



			// Check and call events and swap the buffers
			GLCall(glfwSwapBuffers(window));
			GLCall(glfwPollEvents());
		}
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

	int xMovement = glfwGetKey(window, GLFW_KEY_D) - glfwGetKey(window, GLFW_KEY_A);
	int yMovement = glfwGetKey(window, GLFW_KEY_W) - glfwGetKey(window, GLFW_KEY_S);
	camera.MoveCamera({ xMovement, yMovement });
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	camera.LookCamera({ xpos, ypos });
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ScrollCamera(yoffset);
}