#include "Mesh.h"
#include "../VertexArray/VertexArray.h"
#include "../IndexBuffer/IndexBuffer.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures):
	m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
{
	SetupMesh();
}

Mesh::~Mesh()
{
	delete m_VertexArray;
	delete m_IndexBuffer;
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	for (unsigned int i = 0; i < m_Textures.size(); ++i) {
		GLCall(glActiveTexture(GL_TEXTURE0 + i));
	
		std::string name;
		Texture::TextureType textureType = m_Textures[i].GetTextureType();

		if (textureType == Texture::TextureType::DIFFUSE)
			name = "texture_diffuse" + std::to_string(diffuseNr++);
		else if (textureType == Texture::TextureType::SPECULAR)
			name = "texture_specular" + std::to_string(specularNr++);

		shader.SetFloat("Material." + name, i);
		shader.Bind();
	}

	GLCall(glActiveTexture(GL_TEXTURE0));
	VertexArray va = (*m_VertexArray);
	va.Bind();
	
	va.Unbind();
	
}

void Mesh::SetupMesh()
{
	VertexArray vertexArray;
	VertexBuffer vertexBuffer(m_Vertices.data(), m_Vertices.size());
	VertexBufferLayout vertexBufferLayout;

	vertexBufferLayout.Push<float>(3); // Position
	vertexBufferLayout.Push<float>(3); // Normals
	vertexBufferLayout.Push<float>(2); // Texture Coords
	vertexArray.AddBuffer(vertexBuffer, vertexBufferLayout);

	m_IndexBuffer = new IndexBuffer(m_Indices.data(), m_Indices.size());
}
