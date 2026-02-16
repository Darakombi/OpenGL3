#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class Object {
private:
	VertexArray va;
	VertexBuffer vb;

public:
	Object(const float* vertices, const unsigned int vertexCount,
		const unsigned int* vertexLayout, const unsigned int vertexLayoutCount)
		: vb(vertices, vertexCount * sizeof(float))
	{
		VertexBufferLayout layout;
		for (unsigned int i = 0; i < vertexLayoutCount; i++) {
			layout.Push<float>(vertexLayout[i]);
		}
		va.AddAttribute(vb, layout);
	}

	void Bind() const {
		va.Bind();
	}
	
	void Unbind() const {
		va.Unbind();
	}
};