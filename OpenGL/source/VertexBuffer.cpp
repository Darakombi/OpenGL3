#include "VertexBuffer.h"

#include "glad/glad.h"

VertexBuffer::VertexBuffer(const float* vertices, const unsigned int size)
	: type(GL_ARRAY_BUFFER)
{
	glGenBuffers(1, &ID);
	glBindBuffer(type, ID);
	glBufferData(type, size, vertices, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(const unsigned int* indices, const unsigned int size)
	: type(GL_ELEMENT_ARRAY_BUFFER)
{
	glGenBuffers(1, &ID);
	glBindBuffer(type, ID);
	glBufferData(type, size, indices, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &ID);
}

void VertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VertexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}