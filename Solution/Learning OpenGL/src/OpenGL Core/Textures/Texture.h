#pragma once
#include <string>
class Texture
{
public:
	enum class FileType {
		PNG,
		JPG
	};

	enum class TextureType {
		DIFFUSE,
		SPECULAR
	};

private:
	unsigned int m_ID;
	FileType m_FileType;
	TextureType m_TextureType;
public:
	Texture(const char* pathName, FileType fileType, TextureType textureType);
	~Texture();

	TextureType GetTextureType();

	void Bind() const;
	void Unbind() const;
};

