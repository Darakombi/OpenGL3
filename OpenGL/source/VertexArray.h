#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <array>

class VertexArray
{
private:
	unsigned int ID = 0;
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddVB(const VertexBuffer& vb, const VertexBufferLayout layout) const;
};