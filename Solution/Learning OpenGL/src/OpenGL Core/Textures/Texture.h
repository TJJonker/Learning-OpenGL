#pragma once
#include <string>
class Texture
{
public:
	enum class TextureType {
		PNG,
		JPG
	};

private:
	unsigned int m_ID;

public:
	Texture(const char* pathName, TextureType type);
	~Texture();

	void Bind() const;
	void Unbind() const;
};

