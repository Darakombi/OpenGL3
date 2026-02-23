#include "VertexArray.h"

#include "glad/glad.h"

VertexArray::VertexArray(){
	glGenVertexArrays(1, &ID);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &ID);
}

void VertexArray::Bind() const {
	glBindVertexArray(ID);
}

void VertexArray::Unbind() const {
	glBindVertexArray(0);
}

void VertexArray::AddVB(const VertexBuffer& vb, const VertexBufferLayout layout) const{
	glBindVertexArray(ID);
	vb.Bind();

	const auto& elements = layout.getElements();

	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];

		glVertexAttribPointer(i, element.count, element.type, GL_FALSE, layout.getStride(), (void*)offset);
		glEnableVertexAttribArray(i);
		offset += element.count * VertexBufferElement::getTypeSize(element.type);
	}
}