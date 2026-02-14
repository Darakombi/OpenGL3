#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb_image/stb_image.h"

#include <iostream>

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

void processInput(GLFWwindow* window);

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

	glfwSwapInterval(1);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	std::cout << glGetString(GL_VERSION) << std::endl;

	float vertices[] = {
		// back face
		-0.5, -0.5, -0.5, 0, 0,
		 0.5,  0.5, -0.5, 1, 1,
		 0.5, -0.5, -0.5, 1, 0,
		 0.5,  0.5, -0.5, 1, 1,
		-0.5, -0.5, -0.5, 0, 0,
		-0.5,  0.5, -0.5, 0, 1,

		// front face
		-0.5, -0.5,  0.5, 0, 0,
		 0.5, -0.5,  0.5, 1, 0,
		 0.5,  0.5,  0.5, 1, 1,
		 0.5,  0.5,  0.5, 1, 1,
		-0.5,  0.5,  0.5, 0, 1,
		-0.5, -0.5,  0.5, 0, 0,

		// left face
		-0.5, -0.5, -0.5, 0, 0,
		-0.5, -0.5,  0.5, 1, 0,
		-0.5,  0.5,  0.5, 1, 1,
		-0.5,  0.5,  0.5, 1, 1,
		-0.5,  0.5, -0.5, 0, 1,
		-0.5, -0.5, -0.5, 0, 0,

		// right face
		 0.5, -0.5, -0.5, 0, 0,
		 0.5,  0.5,  0.5, 1, 1,
		 0.5, -0.5,  0.5, 1, 0,
		 0.5,  0.5,  0.5, 1, 1,
		 0.5, -0.5, -0.5, 0, 0,
		 0.5,  0.5, -0.5, 0, 1,

		 // top face
		-0.5, -0.5,  0.5, 0, 0,
		 0.5, -0.5, -0.5, 1, 1,
		 0.5, -0.5,  0.5, 1, 0,
		 0.5, -0.5, -0.5, 1, 1,
		-0.5, -0.5,  0.5, 0, 0,
		-0.5, -0.5, -0.5, 0, 1,

		// bottom face
		-0.5,  0.5,  0.5, 0, 0,
		 0.5,  0.5,  0.5, 1, 0,
		 0.5,  0.5, -0.5, 1, 1,
		 0.5,  0.5, -0.5, 1, 1,
		-0.5,  0.5, -0.5, 0, 1,
		-0.5,  0.5,  0.5, 0, 0,
	};
	VertexArray va;
	va.Bind();

	VertexBuffer vb(vertices, sizeof(vertices));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	va.AddAttribute(vb, layout);

	Shader shader("source/resources/shaders/My.Shader");
	shader.Bind();

	Texture texture("source/resources/textures/sammurray.png");
	Texture texture2("source/resources/textures/lion.png");

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	while (!glfwWindowShouldClose(window)) {

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		texture.Bind(0);
		shader.Uniform1i("tex", 0);
		texture2.Bind(1);
		shader.Uniform1i("tex2", 1);

		glm::mat4 proj = glm::perspective(glm::radians(camera.Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader.UniformMatrix4fv("proj", glm::value_ptr(proj));

		glm::mat4 view = camera.getViewMatrix();
		shader.UniformMatrix4fv("view", glm::value_ptr(view));

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);

			float angle = (i % 3 == 0) ? 20.0f * (float)glfwGetTime() : 0.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			shader.UniformMatrix4fv("model", glm::value_ptr(model));

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