#pragma once

class VertexBuffer
{
private:
	unsigned int ID = 0;
public:
	VertexBuffer(const float* vertices, const unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};