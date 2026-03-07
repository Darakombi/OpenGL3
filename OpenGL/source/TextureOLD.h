#pragma once

#include "glad/glad.h"
#include "stb_image/stb_image.h"

#include <iostream>

class TextureOLD {
private:
	unsigned int ID = 0;
	int Width = 0, Height = 0, NrChannels = 0;
public:
	TextureOLD(const std::string filepath, const GLenum minFilter = GL_NEAREST_MIPMAP_NEAREST, const GLenum magFilter = GL_NEAREST) {
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filepath.c_str(), &Width, &Height, &NrChannels, 0);
		if (data) {
			unsigned int format = (NrChannels == 1) ? (GL_RED) : (NrChannels == 3 ? GL_RGB : GL_RGBA);
			glTexImage2D(GL_TEXTURE_2D, 0, format, Width, Height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else std::cout << "Failed to load texture" << stbi_failure_reason << std::endl;

		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	~TextureOLD() {
		glDeleteTextures(1, &ID);
	}

	void Bind(unsigned int slot = 0) const {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, ID);
	}

	void Unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};