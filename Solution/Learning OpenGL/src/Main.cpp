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
#include <ASSIMP/config.h>
#include "OpenGL Core/Model/Model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Camera camera;

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
		//float vertices[] = {
		//	// positions			// normals				// texture coords
		//	-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
		//	 0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
		//	 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
		//	-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
		//	
		//	-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f, 0.0f,
		//	 0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f, 0.0f,
		//	 0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f, 1.0f,
		//	-0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f, 1.0f,
		//	
		//	-0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
		//	-0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
		//	-0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
		//	-0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
		//	
		//	 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
		//	 0.5f,  0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
		//	 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
		//	 0.5f, -0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
		//	
		//	 0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,
		//	 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
		//	-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f,
		//	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,
		//	
		//	 0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,
		//	 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
		//	-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 1.0f,
		//	-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f


		//};

		//unsigned int indices[] = {
		//	0, 1, 2,
		//	2, 3, 0,

		//	4, 5, 6,
		//	6, 7, 4,

		//	8, 9, 10,
		//	10, 11, 8,

		//	12, 13, 14,
		//	14, 15, 12,

		//	16, 17, 18,
		//	18, 19, 16,

		//	20, 21, 22,
		//	22, 23, 20
		//};

		//glm::vec3 positions[] = {
		//	{0, 0, 0 },
		//	{1, 2, 3},
		//	{2, 3, 1},
		//	{1, 1, .8},
		//	{3, 3, .5},
		//	{2.5, 2, 1.5}
		//};


		///////// Lit object

		//VertexArray vertexArray;
		//int verticesArrayLength = sizeof(vertices) / sizeof(float);
		//VertexBuffer vertexBuffer(vertices, verticesArrayLength * sizeof(float));
		//VertexBufferLayout vertexBufferLayout;

		//vertexBufferLayout.Push<float>(3);
		//vertexBufferLayout.Push<float>(3);
		//vertexBufferLayout.Push<float>(2);
		//vertexArray.AddBuffer(vertexBuffer, vertexBufferLayout);

		//IndexBuffer indexBuffer(indices, 36);

		//Texture diffuseMap("src/textures/container.png", Texture::FileType::PNG, Texture::TextureType::DIFFUSE);
		//Texture specularMap("src/textures/container_specular.png", Texture::FileType::PNG, Texture::TextureType::SPECULAR);
		//glActiveTexture(GL_TEXTURE0);
		//diffuseMap.Bind();
		//glActiveTexture(GL_TEXTURE1);
		//specularMap.Bind();

		Shader shaderLit("src/shaders/backpackVertex.glsl", "src/shaders/backpackFragment.glsl");

		//Model dModel("C:/Users/Tom/Downloads/cube/cube.obj");
		Model dModel("C:/Users/Tom/Downloads/backpack/backpack.obj");


		//////// Light source

		//VertexArray lightVertexArray;
		//lightVertexArray.AddBuffer(vertexBuffer, vertexBufferLayout);
		//Shader lightSourceShader("src/shaders/vertexShaderLight.glsl", "src/shaders/fragmentShaderLight.glsl");
		//glm::vec3 lightPos(0.0f, -5.5f, 2.0f);

		//glm::vec3 pointLightPositions[] = {
		//	glm::vec3(0.7f,  0.2f,  2.0f),
		//	glm::vec3(2.3f, -3.3f, -4.0f),
		//	glm::vec3(-4.0f,  2.0f, -12.0f),
		//	glm::vec3(0.0f,  0.0f, -3.0f)
		//};

		stbi_set_flip_vertically_on_load(true); 
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
			

			shaderLit.Bind();

			//shaderLit.Set3f("material.ambient", 1.0f, 0.5f, 0.31f);
			//shaderLit.SetInt("material.diffuse", 0);
			//shaderLit.SetInt("material.specular", 1);
			//shaderLit.SetFloat("material.shininess", 32.0f);
			//
			//for (int i = 0; i < 4; ++i) {
			//	std::ostringstream str;
			//	str << "pointLights[" << i << "]";
			//	shaderLit.Set3f(str.str() + ".position", pointLightPositions[i]);
			//	shaderLit.SetFloat(str.str() + ".constant", 1.0);
			//	shaderLit.SetFloat(str.str() + ".liinear", 0.09f);
			//	shaderLit.SetFloat(str.str() + ".quadratic", 0.032f);
			//	shaderLit.Set3f(str.str() + ".ambient", 0.2f, 0.2f, 0.2f);
			//	shaderLit.Set3f(str.str() + ".diffuse", 0.5f, 0.5f, 0.5f);
			//	shaderLit.Set3f(str.str() + ".specular", 1.0f, 1.0f, 1.0f);
			//}

			//shaderLit.Set3f("dirLight.direction", camera.GetCameraFront());
			//shaderLit.Set3f("dirLight.ambient", 0.2f, 0.2f, 0.2f);
			//shaderLit.Set3f("dirLight.diffuse", 0.5f, 0.5f, 0.5f); 
			//shaderLit.Set3f("dirLight.specular", 1.0f, 1.0f, 1.0f);

			//shaderLit.Set3f("viewPosition", camera.GetPosition());

			glm::mat4 projection = glm::perspective(glm::radians(camera.GetFOV()), 1000.0f / 1000.0f, 0.1f, 100.0f);
			glm::mat4 viewMatrix = camera.GetView();
			shaderLit.SetMatrix4("projection", projection);
			shaderLit.SetMatrix4("view", viewMatrix);

			//for (unsigned int i = 0; i < 6; ++i) {
				glm::mat4 model = glm::mat4(1.0f);
				//model = glm::translate(model, positions[i]);
			//	float angle = 13 * i;
				//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				//model = glm::scale(model, glm::vec3(0.0025));
				shaderLit.SetMatrix4("model", model);

				dModel.Draw(shaderLit);
			//	Renderer::Draw(vertexArray, indexBuffer, shaderLit);
			//}

			//lightSourceShader.Bind();
			//lightSourceShader.SetMatrix4("projection", projection);
			//lightSourceShader.SetMatrix4("view", viewMatrix);
			//for (int i = 0; i < 4; ++i) {
			//	glm::mat4 model = glm::mat4(1.0f);
			//	model = glm::translate(model, pointLightPositions[i]);
			//	model = glm::scale(model, glm::vec3(0.2f));
			//	lightSourceShader.SetMatrix4("model", model);
			//	Renderer::Draw(lightVertexArray, indexBuffer, lightSourceShader);
			//}

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