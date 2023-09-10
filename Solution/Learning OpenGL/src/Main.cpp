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
#include "vendor/DearImGUI/imgui.h"
#include "vendor/DearImGUI/imgui_impl_glfw.h"
#include "vendor/DearImGUI/imgui_impl_opengl3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Camera camera;

float rotationSpeed = 10;
glm::vec3 lightDirection = { -0.6f, 0.f, -1.f };
glm::vec3 ambientColor = { .1f, .1f, .1f };
glm::vec3 diffuseColor = { .5f, .5f, .5f };
glm::vec3 specularColor = { 1.f, 1.f, 1.f };

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

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	{
		Shader shaderLit("src/shaders/backpackVertex.glsl", "src/shaders/backpackFragment.glsl");
		//Shader shaderLit("src/shaders/vertexShaderLight.glsl", "src/shaders/fragmentShaderLight.glsl");

		Model dModel("C:/Users/Tom/Downloads/backpack/backpack.obj");

		stbi_set_flip_vertically_on_load(true); 
		GLCall(glEnable(GL_DEPTH_TEST));
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");



		// Render loop
		while (!glfwWindowShouldClose(window)) {
			// Inputs
			processInput(window);

			Time::Update();

			// Render instructions
			GLCall(glClearColor(0.1f, 0.1f, 0.125f, 1.0f));
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			

			shaderLit.Bind();

			glm::mat4 projection = glm::perspective(glm::radians(camera.GetFOV()), 1000.0f / 1000.0f, 0.1f, 100.0f);
			glm::mat4 viewMatrix = camera.GetView();
			shaderLit.SetMatrix4("projection", projection);
			shaderLit.SetMatrix4("view", viewMatrix);

			shaderLit.Set3f("directionalLight.direction", lightDirection);
			shaderLit.Set3f("directionalLight.ambient", ambientColor);
			shaderLit.Set3f("directionalLight.diffuse", diffuseColor);
			shaderLit.Set3f("directionalLight.specular", specularColor);
			shaderLit.SetFloat("shininess", 32);
			shaderLit.Set3f("viewPosition", camera.GetPosition());

			glm::mat4 model = glm::mat4(1.0f);
			float angle = Time::TimeSinceStartup() * rotationSpeed;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(0.f, 1.f, 0.f));
			shaderLit.SetMatrix4("model", model);

			dModel.Draw(shaderLit);

			ImGui::Begin("Wow, a new window, fucking awesome");
			ImGui::InputFloat("Rotation speed", &rotationSpeed);
			ImGui::InputFloat3("Light direction", (float*) & lightDirection);
			ImGui::ColorEdit3("Ambient color", (float*)&ambientColor);
			ImGui::ColorEdit3("Diffuse color", (float*)&diffuseColor);
			ImGui::ColorEdit3("Specular color", (float*)&specularColor);
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// Check and call events and swap the buffers
			GLCall(glfwSwapBuffers(window));
			GLCall(glfwPollEvents());
		}
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
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
	//camera.LookCamera({ xpos, ypos });
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ScrollCamera(yoffset);
}