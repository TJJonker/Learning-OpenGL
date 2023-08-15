#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures):
	m_Vertices(vertices), m_Indices(indices), m_Textures(textures), m_VertexArray(), m_IndexBuffer(m_Indices.data(), m_Indices.size())
{
	VertexBuffer vertexBuffer(m_Vertices.data(), m_Vertices.size());
	VertexBufferLayout vertexBufferLayout;

	vertexBufferLayout.Push<float>(3); // Position
	vertexBufferLayout.Push<float>(3); // Normals
	vertexBufferLayout.Push<float>(2); // Texture Coords
	m_VertexArray.AddBuffer(vertexBuffer, vertexBufferLayout);}

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

	m_VertexArray.Bind();
	
	m_VertexArray.Unbind();
	
}