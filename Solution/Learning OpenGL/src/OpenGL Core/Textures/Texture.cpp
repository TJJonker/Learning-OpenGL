#include "Texture.h"
#include "../../Debugging/debugger.h"
#include "../../vendor/stb_lib/stb_lib.h"
#include <iostream>

GLenum Texture::GetDataFormat(int nrChannels) const
{
	if (nrChannels == 1)
		return GL_RED;
	if (nrChannels == 3)
		return GL_RGB;
	return GL_RGBA;
}

Texture::Texture(const std::string& path, const char* fileName, TextureType textureType) :
	m_TextureType(textureType)
{
	GLCall(glGenTextures(1, &m_ID));
	Bind();

	int width, height, nrChannels;
	std::string string = path + "/" + fileName;
	unsigned char* data = stbi_load(string.c_str(), &width, &height, &nrChannels, 0);
	GLenum dataFormat = GetDataFormat(nrChannels);
	if (data) {
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_ID));
}

Texture::TextureType Texture::GetTextureType()
{
	return m_TextureType;
}

void Texture::Bind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}


