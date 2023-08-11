#include "Texture.h"
#include <glad/glad.h>
#include "../../Debugging/debugger.h"
#include "../../vendor/stb_lib/stb_lib.h"
#include <iostream>

Texture::Texture(const char* pathName)
{
	GLCall(glGenTextures(1, &m_ID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	int width, height, nrChannels;
	unsigned char* data = stbi_load(pathName, &width, &height, &nrChannels, 0);
	if (data) {
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_ID));
}

void Texture::Bind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
