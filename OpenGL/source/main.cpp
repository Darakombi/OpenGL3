#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb_image/stb_image.h"

#include <iostream>
#include <string>

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

bool flashlight = true;

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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
	Texture subjectTexture("source/resources/textures/container.png");
	Texture subjectTextureSpecular("source/resources/textures/containerSpecular.png");

	const float lightVertices[] = {
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
	const unsigned int lightLayout[] = { 3, 2 };
	Object light(lightVertices, sizeof(lightVertices) / sizeof(float), lightLayout, sizeof(lightLayout) / sizeof(unsigned int));
	Shader lightShader("source/resources/shaders/Lamp.Shader");
	Texture lightTexture("source/resources/textures/glowstone.png");

	glm::vec3 lightPos(0.0f, 0.0f, -7.0f);
	glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

	float shininess = 32.0f;

	glm::vec3 ambientStrength(0.05f);
	glm::vec3 diffuseStrength(0.8f);
	glm::vec3 specularStrength(1.0f);

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

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		//glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		//glm::vec3(0.0f,  0.0f, -3.0f),
	};
	unsigned int points = 2;

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

			subjectTexture.Bind(0);
			subjectShader.U1i("material.diffuse", 0);
			subjectTextureSpecular.Bind(1);
			subjectShader.U1i("material.specular", 1);
			subjectShader.U1f("material.shininess", shininess);

			subjectShader.U3fv("u_ViewPos", glm::value_ptr(camera.getPosition()));

			subjectShader.U3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
			subjectShader.U3f("dirLight.ambient", 0.05f, 0.05f, 0.05f);
			subjectShader.U3f("dirLight.diffuse", 0.8f, 0.8f, 0.8f);
			subjectShader.U3f("dirLight.specular", 1.0f, 1.0f, 1.0f);

			for (unsigned int i = 0; i < points; i++) {
				std::string name = "pointLights[" + std::to_string(i) + "]";

				subjectShader.U3fv((name + ".position").c_str(), glm::value_ptr(pointLightPositions[i]));
				subjectShader.U3f((name + ".ambient").c_str(), 0.05f, 0.05f, 0.05f);
				subjectShader.U3f((name + ".diffuse").c_str(), 0.8f, 0.8f, 0.8f);
				subjectShader.U3f((name + ".specular").c_str(), 1.0f, 1.0f, 1.0f);
				subjectShader.U1f((name + ".constant").c_str(), 1.0f);
				subjectShader.U1f((name + ".linear").c_str(), 0.09f);
				subjectShader.U1f((name + ".quadratic").c_str(), 0.032f);
			}

			subjectShader.U3fv("spotLight.position", glm::value_ptr(camera.getPosition()));
			subjectShader.U3fv("spotLight.direction", glm::value_ptr(camera.getFront()));
			subjectShader.U3f("spotLight.ambient", 0.0f, 0.0f, 0.0f);
			subjectShader.U3f("spotLight.diffuse", flashlight ? 1.0f : 0.0f, flashlight ? 1.0f : 0.0f, flashlight ? 1.0f : 0.0f);
			subjectShader.U3f("spotLight.specular", 1.0f, 1.0f, 1.0f);
			subjectShader.U1f("spotLight.constant", 1.0f);
			subjectShader.U1f("spotLight.linear", 0.045f);
			subjectShader.U1f("spotLight.quadratic", 0.0075f);
			subjectShader.U1f("spotLight.innerCutoff", cos(glm::radians(12.5f)));
			subjectShader.U1f("spotLight.outerCutoff", cos(glm::radians(17.5f)));

			subjectShader.UMat4fv("proj", glm::value_ptr(proj));
			subjectShader.UMat4fv("view", glm::value_ptr(view));
			for (unsigned int i = 0; i < 10; i++)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, cubePositions[i]);
				float angle = 20.0f * i;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				subjectShader.UMat4fv("model", glm::value_ptr(model));

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		{
			light.Bind();
			lightShader.Bind();

			lightTexture.Bind(0);
			lightShader.U1i("u_TextureColor", 0);

			lightShader.UMat4fv("proj", glm::value_ptr(proj));
			lightShader.UMat4fv("view", glm::value_ptr(view));
			for (unsigned int i = 0; i < points; i++) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, pointLightPositions[i]);
				model = glm::scale(model, glm::vec3(0.2f));
				lightShader.UMat4fv("model", glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
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
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
		std::cout << camera.getFov() << std::endl;
	}
	else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		flashlight = !flashlight;
	}
}