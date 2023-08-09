#pragma once
#include <string>
class Texture
{
private:
	unsigned int m_ID;

public:
	Texture(const char* pathName);
	~Texture();

	void Bind() const;
	void Unbind() const;
};

