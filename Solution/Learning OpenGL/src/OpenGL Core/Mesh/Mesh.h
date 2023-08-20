#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "../Textures/Texture.h"
#include "../Shader/Shader.h"
#include "../VertexArray/VertexArray.h"
#include "../IndexBuffer/IndexBuffer.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normals;
	glm::vec2 TextureCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

class Mesh
{
private:
	VertexArray m_VertexArray;
	IndexBuffer m_IndexBuffer;
	
public:
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	void Draw(Shader& shader);
};

