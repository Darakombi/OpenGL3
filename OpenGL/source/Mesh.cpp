#include "Mesh.h"

#include <string>

Mesh::Mesh(const std::vector<MeshVertex> vertices,
	const std::vector<unsigned int> indices,
	const std::vector<MeshTexture> textures)
	: Vertices(vertices),
	Indices(indices),
	Textures(textures)
{
	setupMesh();
}

void Mesh::setupMesh(){
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(MeshVertex), &Vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(GL_ARRAY_BUFFER, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(GL_ARRAY_BUFFER, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, Normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(GL_ARRAY_BUFFER, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, TexCoord));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(Indices), &Indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader) const {
	unsigned int diffuseArg = 1;
	unsigned int specularArg = 1;

	for (int i = 0; i < Textures.size(); i++) {

		glActiveTexture(GL_TEXTURE0 + i);

		std::string arg;

		const std::string type = Textures[i].type;
		if (type == "diffuse") {
			arg = std::to_string(diffuseArg++);
		}
		else if (type == "specular") {
			arg = std::to_string(specularArg++);
		}

		std::string name = "material.texture" + type + arg;

		shader.U1i(name.c_str(), i);
		glBindTexture(GL_TEXTURE_2D, Textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, sizeof(Indices), GL_FLOAT, 0);
	glBindVertexArray(0);
}