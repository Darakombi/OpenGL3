#ifndef MESH_H
#define MESH_H

#include "Shader.h"

#include "glm/glm.hpp"

#include "string"
#include <vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {

public:
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<Texture> Textures;

	Mesh(const std::vector<Vertex> vertices,
		 const std::vector<unsigned int> indices,
		 const std::vector<Texture> textures)
		: Vertices(vertices),
		Indices(indices),
		Textures(textures)
	{
		setupMesh();
	}

	void Draw(const Shader& shader) const {
		unsigned int diffuseArg = 1;
		unsigned int specularArg = 1;

		for (unsigned int i = 0; i < Textures.size(); i++) {

			glActiveTexture(GL_TEXTURE0 + i);

			std::string arg;
			const std::string type = Textures[i].type;
			if (type == "diffuse") {
				arg = std::to_string(diffuseArg++);
			}
			else if (type == "specular") {
				arg = std::to_string(specularArg++);
			}

			std::string name = "material." + type + arg;

			shader.U1i(name.c_str(), i);
			glBindTexture(GL_TEXTURE_2D, Textures[i].id);
		}

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(Indices.size()), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}

private:
	unsigned int VAO, IBO, VBO;

	void setupMesh() {
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), Vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));
		glEnableVertexAttribArray(2);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

};

#endif