#pragma once
#include "../Shader/Shader.h"
#include "../Textures/Texture.h"
#include "../Mesh/Mesh.h"

class TexturePlane
{
private:
	Mesh* mesh;
public:
	TexturePlane(std::shared_ptr<Texture> texture);
	~TexturePlane();

	void Draw(Shader& shader);
};

