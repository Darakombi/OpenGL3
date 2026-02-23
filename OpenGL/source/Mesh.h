#pragma once

#include "Shader.h"

#include "glm/glm.hpp"

#include <iostream>
#include <vector>

struct MeshVertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
};

struct MeshTexture {
	unsigned int id;
	std::string type;
};

class Mesh {

public:
	std::vector<MeshVertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<MeshTexture> Textures;

	Mesh(const std::vector<MeshVertex> vertices,
		const std::vector<unsigned int> indices,
		const std::vector<MeshTexture> textures);

	void Draw(Shader& shader) const;

private:
	unsigned int VAO, IBO, VBO;
	void setupMesh();
};