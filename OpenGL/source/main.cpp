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

	const float subjectVertices[] = {
		// back face
		-0.5, -0.5, -0.5,    0.0f,  0.0f, -1.0f,
		 0.5,  0.5, -0.5,    0.0f,  0.0f, -1.0f,
		 0.5, -0.5, -0.5,    0.0f,  0.0f, -1.0f,
		 0.5,  0.5, -0.5,    0.0f,  0.0f, -1.0f,
		-0.5, -0.5, -0.5,    0.0f,  0.0f, -1.0f,
		-0.5,  0.5, -0.5,    0.0f,  0.0f, -1.0f,

		// front face
		-0.5, -0.5,  0.5,    0.0f,  0.0f,  1.0f,
		 0.5, -0.5,  0.5,    0.0f,  0.0f,  1.0f,
		 0.5,  0.5,  0.5,    0.0f,  0.0f,  1.0f,
		 0.5,  0.5,  0.5,    0.0f,  0.0f,  1.0f,
		-0.5,  0.5,  0.5,    0.0f,  0.0f,  1.0f,
		-0.5, -0.5,  0.5,    0.0f,  0.0f,  1.0f,

		// left face
		-0.5, -0.5, -0.5,   -1.0f,  0.0f,  0.0f,
		-0.5, -0.5,  0.5,   -1.0f,  0.0f,  0.0f,
		-0.5,  0.5,  0.5,   -1.0f,  0.0f,  0.0f,
		-0.5,  0.5,  0.5,   -1.0f,  0.0f,  0.0f,
		-0.5,  0.5, -0.5,   -1.0f,  0.0f,  0.0f,
		-0.5, -0.5, -0.5,   -1.0f,  0.0f,  0.0f,

		// right face
		 0.5, -0.5, -0.5,    1.0f,  0.0f,  0.0f,
		 0.5,  0.5,  0.5,    1.0f,  0.0f,  0.0f,
		 0.5, -0.5,  0.5,    1.0f,  0.0f,  0.0f,
		 0.5,  0.5,  0.5,    1.0f,  0.0f,  0.0f,
		 0.5, -0.5, -0.5,    1.0f,  0.0f,  0.0f,
		 0.5,  0.5, -0.5,    1.0f,  0.0f,  0.0f,

		 // bottom face
		 -0.5, -0.5,  0.5,    0.0f, -1.0f,  0.0f,
		  0.5, -0.5,  0.5,    0.0f, -1.0f,  0.0f,
		  0.5, -0.5, -0.5,    0.0f, -1.0f,  0.0f,
		  0.5, -0.5, -0.5,    0.0f, -1.0f,  0.0f,
		 -0.5, -0.5, -0.5,    0.0f, -1.0f,  0.0f,
		 -0.5, -0.5,  0.5,    0.0f, -1.0f,  0.0f,

		 // top face
		 -0.5,  0.5,  0.5,    0.0f,  1.0f,  0.0f,
		  0.5,  0.5, -0.5,    0.0f,  1.0f,  0.0f,
		  0.5,  0.5,  0.5,    0.0f,  1.0f,  0.0f,
		  0.5,  0.5, -0.5,    0.0f,  1.0f,  0.0f,
		 -0.5,  0.5,  0.5,    0.0f,  1.0f,  0.0f,
		 -0.5,  0.5, -0.5,    0.0f,  1.0f,  0.0f,
	};
	const unsigned int subjectLayout[] = { 3, 3 };
	Object subject(subjectVertices, sizeof(subjectVertices) / sizeof(float), subjectLayout, sizeof(subjectLayout) / sizeof(unsigned int));
	Shader subjectShader("source/resources/shaders/Main.Shader");
	glm::vec3 subjectColor(1.0f, 0.5f, 0.31f);

	Object subject2(subjectVertices, sizeof(subjectVertices) / sizeof(float), subjectLayout, sizeof(subjectLayout) / sizeof(unsigned int));
	glm::vec3 subjectColor2(0.2f, 0.5f, 0.31f);

	const float lampVertices[] = {
		// back face
		-0.5, -0.5, -0.5,
		 0.5,  0.5, -0.5,
		 0.5, -0.5, -0.5,
		 0.5,  0.5, -0.5,
		-0.5, -0.5, -0.5,
		-0.5,  0.5, -0.5,

		// front face
		-0.5, -0.5,  0.5,
		 0.5, -0.5,  0.5,
		 0.5,  0.5,  0.5,
		 0.5,  0.5,  0.5,
		-0.5,  0.5,  0.5,
		-0.5, -0.5,  0.5,

		// left face
		-0.5, -0.5, -0.5,
		-0.5, -0.5,  0.5,
		-0.5,  0.5,  0.5,
		-0.5,  0.5,  0.5,
		-0.5,  0.5, -0.5,
		-0.5, -0.5, -0.5,

		// right face
		 0.5, -0.5, -0.5,
		 0.5,  0.5,  0.5,
		 0.5, -0.5,  0.5,
		 0.5,  0.5,  0.5,
		 0.5, -0.5, -0.5,
		 0.5,  0.5, -0.5,

		 // top face
		-0.5, -0.5,  0.5,
		 0.5, -0.5, -0.5,
		 0.5, -0.5,  0.5,
		 0.5, -0.5, -0.5,
		-0.5, -0.5,  0.5,
		-0.5, -0.5, -0.5,

		// bottom face
		-0.5,  0.5,  0.5,
		 0.5,  0.5,  0.5,
		 0.5,  0.5, -0.5,
		 0.5,  0.5, -0.5,
		-0.5,  0.5, -0.5,
		-0.5,  0.5,  0.5,
	};
	const unsigned int lampLayout[] = { 3 };
	Object lamp(lampVertices, sizeof(lampVertices) / sizeof(float), lampLayout, sizeof(lampLayout) / sizeof(unsigned int));
	Shader lampShader("source/resources/shaders/Lamp.Shader");
	glm::vec3 lampPos(1.2f, 1.0f, 2.0f);
	glm::vec3 lampColor(1.0f, 1.0f, 1.0f);

	float transX = 0.0f;
	float transXAdd = 0.005f;

	while (!glfwWindowShouldClose(window)) {

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 proj = glm::perspective(glm::radians(camera.Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 model(1.0f);

		{
			subject.Bind();
			subjectShader.Bind();

			subjectShader.Uniform3fv("u_ObjectColor", glm::value_ptr(subjectColor));
			subjectShader.Uniform3fv("u_LightColor", glm::value_ptr(lampColor));
			subjectShader.Uniform3fv("u_LightPos", glm::value_ptr(lampPos));
			subjectShader.Uniform3fv("u_ViewPos", glm::value_ptr(camera.getPosition()));

			subjectShader.UniformMatrix4fv("proj", glm::value_ptr(proj));
			subjectShader.UniformMatrix4fv("view", glm::value_ptr(view));
			//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(transX, 0.0f, 0.0f));
			subjectShader.UniformMatrix4fv("model", glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);

			subject2.Bind();
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
			subjectShader.UniformMatrix4fv("model", glm::value_ptr(model));
			subjectShader.Uniform3fv("u_ObjectColor", glm::value_ptr(subjectColor2));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		transX = std::sin(glfwGetTime()) + 1.0f;
		transX += transXAdd;

		{
			lamp.Bind();
			lampShader.Bind();

			lampShader.Uniform3fv("u_Color", glm::value_ptr(lampColor));

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