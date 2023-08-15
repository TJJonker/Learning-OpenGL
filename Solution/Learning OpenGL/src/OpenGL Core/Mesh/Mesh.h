#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "../Textures/Texture.h"
#include "../Shader/Shader.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 TextureCoords;
};

class Mesh
{
private:
	VertexArray*				m_VertexArray;
	IndexBuffer*				m_IndexBuffer;
	std::vector<Vertex>			m_Vertices;
	std::vector<unsigned int>	m_Indices;
	std::vector<Texture>		m_Textures;
	
	void SetupMesh();

public:
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
	~Mesh();
	void Draw(Shader& shader);
};

