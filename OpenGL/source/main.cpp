#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb_image/stb_image.h"

#include <iostream>

#include "Object.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double positionX, double positionY);
void scroll_callback(GLFWwindow* window, double offsetX, double offsetY);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void processInput(GLFWwindow* window);

inline static const float time() { return (float)glfwGetTime(); }

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool firstMouse = true;
float lastMouseX = (float)SCR_WIDTH / 2.0f;
float lastMouseY = (float)SCR_HEIGHT / 2.0f;

int main() {

	if (!glfwInit())
		return 1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lookin for my Kamakiri", nullptr, nullptr);
	if (window == NULL) {
		std::cout << "Failed to initialize window" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return 1;
	}

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwSwapInterval(1);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	std::cout << glGetString(GL_VERSION) << std::endl;

	const float subjectVertices[] = {
		// back face
		-0.5, -0.5, -0.5,	 0,  0, -1,		0, 0,
		 0.5,  0.5, -0.5,	 0,  0, -1,		1, 1,
		 0.5, -0.5, -0.5,	 0,  0, -1,		1, 0,
		 0.5,  0.5, -0.5,	 0,  0, -1,		1, 1,
		-0.5, -0.5, -0.5,	 0,  0, -1,		0, 0,
		-0.5,  0.5, -0.5,	 0,  0, -1,		0, 1,

		// front face
		-0.5, -0.5,  0.5,	 0,  0,  1,		0, 0,
		 0.5, -0.5,  0.5,	 0,  0,  1,		1, 0,
		 0.5,  0.5,  0.5,	 0,  0,  1,		1, 1,
		 0.5,  0.5,  0.5,	 0,  0,  1,		1, 1,
		-0.5,  0.5,  0.5,	 0,  0,  1,		0, 1,
		-0.5, -0.5,  0.5,	 0,  0,  1,		0, 0,

		// left face			 	 
		-0.5, -0.5, -0.5,	-1,  0,  0,		0, 0,
		-0.5,  0.5, -0.5,	-1,  0,  0,		0, 1,
		-0.5,  0.5,  0.5,	-1,  0,  0,		1, 1,
		-0.5, -0.5, -0.5,	-1,  0,  0,		0, 0,
		-0.5,  0.5,  0.5,	-1,  0,  0,		1, 1,
		-0.5, -0.5,  0.5,	-1,  0,  0,		1, 0,

		// right face			 	 
		 0.5, -0.5, -0.5,	 1,  0,  0,		1, 0,
		 0.5,  0.5, -0.5,	 1,  0,  0,		1, 1,
		 0.5,  0.5,  0.5,	 1,  0,  0,		0, 1,
		 0.5, -0.5, -0.5,	 1,  0,  0,		1, 0,
		 0.5, -0.5,  0.5,	 1,  0,  0,		0, 0,
		 0.5,  0.5,  0.5,	 1,  0,  0,		0, 1,

		 // bottom face			 	 
		-0.5, -0.5,  0.5,	 0, -1,  0,		0, 1,
		 0.5, -0.5,  0.5,	 0, -1,  0,		1, 1,
		 0.5, -0.5, -0.5,	 0, -1,  0,		1, 0,
		 0.5, -0.5, -0.5,	 0, -1,  0,		1, 0,
		-0.5, -0.5, -0.5,	 0, -1,  0,		0, 0,
		-0.5, -0.5,  0.5,	 0, -1,  0,		0, 1,

		// top face
		-0.5,  0.5, -0.5,	 0,  1,  0,		0, 0,
		 0.5,  0.5, -0.5,	 0,  1,  0,		1, 0,
		 0.5,  0.5,  0.5,	 0,  1,  0,		1, 1,
		-0.5,  0.5, -0.5,	 0,  1,  0,		0, 0,
		-0.5,  0.5,  0.5,	 0,  1,  0,		0, 1,
		 0.5,  0.5,  0.5,	 0,  1,  0,		1, 1
	};
	const unsigned int subjectLayout[] = { 3, 3, 2 };
	Object subject(subjectVertices, sizeof(subjectVertices) / sizeof(float), subjectLayout, sizeof(subjectLayout) / sizeof(unsigned int));
	Shader subjectShader("source/resources/shaders/Main.Shader");
	Texture subjectTexture("source/resources/textures/amethyst_block.png");
	Texture subjectTextureSpecular("source/resources/textures/amethyst_blockSpecular.png");

	const float lampVertices[] = {
		// back face
		-0.5, -0.5, -0.5,	 0, 0,
		 0.5,  0.5, -0.5,	 1, 1,
		 0.5, -0.5, -0.5,	 1, 0,
		 0.5,  0.5, -0.5,	 1, 1,
		-0.5, -0.5, -0.5,	 0, 0,
		-0.5,  0.5, -0.5,	 0, 1,

		// front face
		-0.5, -0.5,  0.5,	 0, 0,
		 0.5, -0.5,  0.5,	 1, 0,
		 0.5,  0.5,  0.5,	 1, 1,
		 0.5,  0.5,  0.5,	 1, 1,
		-0.5,  0.5,  0.5,	 0, 1,
		-0.5, -0.5,  0.5,	 0, 0,

		// left face		
		-0.5, -0.5, -0.5,	-0, 0,
		-0.5,  0.5, -0.5,	-0, 1,
		-0.5,  0.5,  0.5,	-1, 1,
		-0.5, -0.5, -0.5,	-0, 0,
		-0.5,  0.5,  0.5,	-1, 1,
		-0.5, -0.5,  0.5,	-1, 0,

		// right face		
		 0.5, -0.5, -0.5,	 1, 0,
		 0.5,  0.5, -0.5,	 1, 1,
		 0.5,  0.5,  0.5,	 0, 1,
		 0.5, -0.5, -0.5,	 1, 0,
		 0.5, -0.5,  0.5,	 0, 0,
		 0.5,  0.5,  0.5,	 0, 1,

		 // bottom face		
		-0.5, -0.5,  0.5,	 0, 1,
		 0.5, -0.5,  0.5,	 1, 1,
		 0.5, -0.5, -0.5,	 1, 0,
		 0.5, -0.5, -0.5,	 1, 0,
		-0.5, -0.5, -0.5,	 0, 0,
		-0.5, -0.5,  0.5,	 0, 1,

		// top face
		-0.5,  0.5, -0.5,	 0, 0,
		 0.5,  0.5, -0.5,	 1, 0,
		 0.5,  0.5,  0.5,	 1, 1,
		-0.5,  0.5, -0.5,	 0, 0,
		-0.5,  0.5,  0.5,	 0, 1,
		 0.5,  0.5,  0.5,	 1, 1
	};
	const unsigned int lampLayout[] = { 3, 2 };
	Object lamp(lampVertices, sizeof(lampVertices) / sizeof(float), lampLayout, sizeof(lampLayout) / sizeof(unsigned int));
	Shader lampShader("source/resources/shaders/Lamp.Shader");
	Texture lampTexture("source/resources/textures/glowstone.png");

	glm::vec3 lampPos(0.0f, 1.0f, 1.0f);
	glm::vec3 lampColor(1.0f, 1.0f, 1.0f);

	float shininess = 32.0f;

	glm::vec3 ambientStrength(0.1f);
	glm::vec3 diffuseStrength(1.0f);
	glm::vec3 specularStrength(10.0f);

	while (!glfwWindowShouldClose(window)) {

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 proj = glm::perspective(glm::radians(camera.Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 model(1.0f);
		model = glm::scale(model, glm::vec3(0.6f));

		lampPos = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(deltaTime * 15.0f), glm::vec3(1, 0, 0)) * glm::vec4(lampPos, 1.0f));
		lampPos.x += (sin(glfwGetTime() * 10.0) * 0.0005f);

		{
			subject.Bind();
			subjectShader.Bind();

			subjectTexture.Bind(0);
			subjectShader.Uniform1i("material.diffuse", 0);
			subjectTextureSpecular.Bind(1);
			subjectShader.Uniform1i("material.specular", 1);

			{
				subjectShader.Uniform1f("material.shininess", shininess);

				subjectShader.Uniform3fv("light.position", glm::value_ptr(lampPos));
				subjectShader.Uniform3fv("light.ambient", glm::value_ptr(ambientStrength));
				subjectShader.Uniform3fv("light.diffuse", glm::value_ptr(diffuseStrength));
				subjectShader.Uniform3fv("light.specular", glm::value_ptr(specularStrength));

				subjectShader.Uniform3fv("u_ViewPos", glm::value_ptr(camera.getPosition()));
			}

			subjectShader.UniformMatrix4fv("proj", glm::value_ptr(proj));
			subjectShader.UniformMatrix4fv("view", glm::value_ptr(view));
			subjectShader.UniformMatrix4fv("model", glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		{
			lamp.Bind();
			lampShader.Bind();

			lampTexture.Bind(0);
			lampShader.Uniform1i("u_TextureColor", 0);

			lampShader.UniformMatrix4fv("proj", glm::value_ptr(proj));
			lampShader.UniformMatrix4fv("view", glm::value_ptr(view));
			model = glm::mat4(1.0f);
			model = glm::translate(model, lampPos);
			model = glm::scale(model, glm::vec3(0.2f));
			lampShader.UniformMatrix4fv("model", glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	float cameraSpeed = 2.5 * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.processKeys(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.processKeys(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.processKeys(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.processKeys(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		camera.processKeys(DOWN, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera.processKeys(UP, deltaTime);
	}
}

void mouse_callback(GLFWwindow* window, double positionX, double positionY) {

	if (firstMouse) {
		lastMouseX = positionX;
		lastMouseY = positionY;
		firstMouse = false;
	}

	float offsetX = positionX - lastMouseX;
	float offsetY = lastMouseY - positionY;

	lastMouseX = positionX;
	lastMouseY = positionY;

	camera.processMouse(offsetX, offsetY);
}

void scroll_callback(GLFWwindow* window, double offsetX, double offsetY) {
	camera.processScroll(offsetY);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		camera.getFacing();
	}
}