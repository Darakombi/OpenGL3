#pragma once

#include <iostream>

//struct Params {
//	GLint s_wrap;
//	GLint t_wrap;
//	GLint min_filter;
//	GLint mag_filter;
//};

class Texture {
private:
	unsigned int ID = 0;
	int Width = 0, Height = 0, Channels = 0;
public:
	//Texture(const std::string filepath, const Params params = { GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR });
	Texture(const std::string filepath, const bool near = true);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
};