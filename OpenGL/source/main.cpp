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

void framebuffer_size_callback(GLFWwindow*, int width, int height);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {

	if (!glfwInit())
		return 1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MrX", nullptr, nullptr);
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

	/*unsigned int indices[] = {
		0, 1, 3,
		1, 3, 2
	};
	IndexBuffer ib(indices, sizeof(indices));
	ib.Bind();*/

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glm::mat4 model(1.0f);
		//model = glm::rotate(model, glm::radians((float)glfwGetTime()) * 25, glm::vec3(1.0f, 0.0f, 0.0f));

		texture.Bind(0);
		shader.Uniform1i("tex", 0);
		texture2.Bind(1);
		shader.Uniform1i("tex2", 1);

		glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		glm::mat4 view(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		shader.UniformMatrix4fv("proj", glm::value_ptr(proj));
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

		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}