#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb_image/stb_image.h"

#include <iostream>
#include <string>

#include "Shader.h"
#include "Model.h"
#include "Camera.h"

#include "TextureOLD.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double positionX, double positionY);
void scroll_callback(GLFWwindow* window, double offsetX, double offsetY);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool firstMouse = true;
float lastMouseX = (float)SCR_WIDTH / 2.0f;
float lastMouseY = (float)SCR_HEIGHT / 2.0f;

bool flashlight = true;

int main() {

	if (!glfwInit())
		return 1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Looking for my Kamakiri", nullptr, nullptr);
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
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwSwapInterval(1);
	stbi_set_flip_vertically_on_load(true);

	std::cout << glGetString(GL_VERSION) << std::endl;

	Shader shader("source/resources/shaders/Main.Shader");
	Model testModel("source/resources/textures/backpack/backpack.obj");

	const float vertices[] = {
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
	
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//glm::vec3 lightPos(0.0f, 5.0f, -12.0f);

	Shader lightShader("source/resources/shaders/Light.shader");
	TextureOLD lightTexture("source/resources/textures/glowstone.png");

	while (!glfwWindowShouldClose(window)) {

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Bind();

		glm::mat4 proj = glm::perspective(glm::radians(camera.Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 model(1.0f);

		glm::vec3 lightPos(1.0f);
		float radius = 12.0f;
		lightPos.y = 6.0f;
		lightPos.x = cos(glfwGetTime()) * radius;
		lightPos.z = sin(glfwGetTime()) * radius;

		shader.UMat4fv("proj", glm::value_ptr(proj));
		shader.UMat4fv("view", glm::value_ptr(view));
		shader.UMat4fv("model", glm::value_ptr(model));

		shader.U3fv("u_ViewPos", glm::value_ptr(camera.getPosition()));

		shader.U3fv("light.position", glm::value_ptr(lightPos));

		shader.U3f("light.ambient", 0.2, 0.2, 0.2);
		shader.U3f("light.diffuse", 1.0, 1.0, 1.0);
		shader.U3f("light.specular", 0.5, 0.5, 0.5);

		shader.U1f("light.constant", 1.0);
		shader.U1f("light.linear", 0.022);
		shader.U1f("light.quadratic", 0.0019);

		testModel.Draw(shader);

		glBindVertexArray(vao);
		lightShader.Bind();

		lightTexture.Bind(0);
		lightShader.U1i("u_Tex1", 0);

		lightShader.UMat4fv("proj", glm::value_ptr(proj));
		lightShader.UMat4fv("view", glm::value_ptr(view));

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		lightShader.UMat4fv("model", glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);

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
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
		std::cout << camera.getFov() << std::endl;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		flashlight = !flashlight;
	}
}