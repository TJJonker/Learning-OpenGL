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

float rotationSpeed = 38;
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
		Shader dShader("src/shaders/backpackVertex.glsl", "src/shaders/backpackFragment.glsl");
		Shader eShader("src/shaders/backpackVertex.glsl", "src/shaders/backpackFragment.glsl");
		Shader oShader("src/shaders/Stencil_Testing/SingleColorVertex.glsl", "src/shaders/Stencil_Testing/SingleColorFragment.glsl");
		
		//Shader dShader("src/shaders/Depth_Testing/DepthVertex.glsl", "src/shaders/Depth_Testing/DepthFragment.glsl");
		//Shader eShader("src/shaders/Depth_Testing/DepthVertex.glsl", "src/shaders/Depth_Testing/DepthFragment.glsl");

		Model dModel("C:/Users/Tom/Downloads/Simple_City_SourceFiles/SourceFiles/Combined/SC_Bld_03_Dark.obj");
		Model eModel("C:/Users/Tom/Downloads/Simple_City_SourceFiles/SourceFiles/Combined/SC_Prop_Chinatown_Entrance.obj");

		stbi_set_flip_vertically_on_load(true); 
		GLCall(glEnable(GL_DEPTH_TEST));
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
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
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();


			{
				glStencilMask(0x00);
				// China town entrance
				eShader.Bind();

				glm::mat4 projection = glm::perspective(glm::radians(camera.GetFOV()), 1000.0f / 1000.0f, 0.1f, 100.0f);
				glm::mat4 viewMatrix = camera.GetView();
				eShader.SetMatrix4("projection", projection);
				eShader.SetMatrix4("view", viewMatrix);

				eShader.Set3f("directionalLight.direction", lightDirection);
				eShader.Set3f("directionalLight.ambient", ambientColor);
				eShader.Set3f("directionalLight.diffuse", diffuseColor);
				eShader.Set3f("directionalLight.specular", specularColor);
				eShader.SetFloat("shininess", 32);
				eShader.Set3f("viewPosition", camera.GetPosition());

				glm::mat4 model = glm::mat4(1.0f);
				float angle = rotationSpeed * 2;
				//float angle = Time::TimeSinceStartup() * rotationSpeed;
				model = glm::translate(model, glm::vec3(0.f, -0.25f, 7.25f));
				model = glm::scale(model, glm::vec3(.025f, .025f, .025f));
				model = glm::rotate(model, glm::radians(angle), glm::vec3(0.f, 1.f, 0.f));
				eShader.SetMatrix4("model", model);

				eModel.Draw(eShader);
			}
			
			{
				glStencilFunc(GL_ALWAYS, 1, 0xFF);
				glStencilMask(0xFF);

				// Skyscraper 
				dShader.Bind();
				
				glm::mat4 projection = glm::perspective(glm::radians(camera.GetFOV()), 1000.0f / 1000.0f, 0.1f, 100.0f);
				glm::mat4 viewMatrix = camera.GetView();
				dShader.SetMatrix4("projection", projection);
				dShader.SetMatrix4("view", viewMatrix);

				dShader.Set3f("directionalLight.direction", lightDirection);
				dShader.Set3f("directionalLight.ambient", ambientColor);
				dShader.Set3f("directionalLight.diffuse", diffuseColor);
				dShader.Set3f("directionalLight.specular", specularColor);
				dShader.SetFloat("shininess", 32);
				dShader.Set3f("viewPosition", camera.GetPosition());

				glm::mat4 model = glm::mat4(1.0f);
				float angle = rotationSpeed;
				//float angle = Time::TimeSinceStartup() * rotationSpeed;
				model = glm::translate(model, glm::vec3(3.f, -2.f, -5.f));
				model = glm::scale(model, glm::vec3(.05f, .05f, .05f));
				model = glm::rotate(model, glm::radians(angle), glm::vec3(0.f, 1.f, 0.f));
				dShader.SetMatrix4("model", model);

				dModel.Draw(dShader);
			}

			// Draw Skyscraper outline
			{
				glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
				glStencilMask(0x00);
				glDisable(GL_DEPTH_TEST);

				// Skyscraper 
				oShader.Bind();

				glm::mat4 projection = glm::perspective(glm::radians(camera.GetFOV()), 1000.0f / 1000.0f, 0.1f, 100.0f);
				glm::mat4 viewMatrix = camera.GetView();
				oShader.SetMatrix4("projection", projection);
				oShader.SetMatrix4("view", viewMatrix);

				oShader.Set3f("directionalLight.direction", lightDirection);
				oShader.Set3f("directionalLight.ambient", ambientColor);
				oShader.Set3f("directionalLight.diffuse", diffuseColor);
				oShader.Set3f("directionalLight.specular", specularColor);
				oShader.SetFloat("shininess", 32);
				oShader.Set3f("viewPosition", camera.GetPosition());

				glm::mat4 model = glm::mat4(1.0f);
				float angle = rotationSpeed;
				//float angle = Time::TimeSinceStartup() * rotationSpeed;
				model = glm::translate(model, glm::vec3(3.f, -2.f, -5.f));
				model = glm::scale(model, glm::vec3(.051f, .051f, .051f));
				model = glm::rotate(model, glm::radians(angle), glm::vec3(0.f, 1.f, 0.f));
				oShader.SetMatrix4("model", model);

				dModel.Draw(oShader);
			}

			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
			glEnable(GL_DEPTH_TEST);

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