#pragma once

class VertexBuffer
{
private:
	unsigned int ID = 0;
	unsigned int type;
public:
	VertexBuffer(const float* vertices, const unsigned int size);
	VertexBuffer(const unsigned int* indices, const unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};