#pragma once
#include <string>
#include <glad/glad.h>
class Texture
{
public:

	enum class TextureType {
		DIFFUSE = 0,
		SPECULAR = 1,
		NORMAL = 2,
		HEIGHT = 3
	};

private:
	unsigned int m_ID;
	TextureType m_TextureType;

	GLenum GetDataFormat(int nrChannels) const;

public:
	Texture(const std::string& path, const char* pathName, TextureType textureType);
	~Texture();

	TextureType GetTextureType();

	void Bind() const;
	void Unbind() const;
};

