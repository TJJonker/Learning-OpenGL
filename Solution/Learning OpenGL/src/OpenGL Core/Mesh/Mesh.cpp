#include "Mesh.h"
#include "../Renderer/Renderer.h"


Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<std::shared_ptr<Texture>>& textures):
	m_VertexArray(), m_IndexBuffer(indices.data(), indices.size()), m_Textures(textures)
{
	// Create vertexBufferLayout
	VertexBuffer vertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex));

	VertexBufferLayout vertexBufferLayout;

	// Allocate space for certain memory
	vertexBufferLayout.Push<float>(3); // Position
	vertexBufferLayout.Push<float>(3); // Normals
	vertexBufferLayout.Push<float>(2); // TextureCoords
	vertexBufferLayout.Push<float>(3); // Tangent
	vertexBufferLayout.Push<float>(3); // Bitangent

	static_assert(sizeof(Vertex) == 56, "Update vertex buffer layout");

	// Bind it to the vertexArray
	m_VertexArray.AddBuffer(vertexBuffer, vertexBufferLayout);
	m_VertexArray.Unbind();
}

void Mesh::Draw(Shader& shader)
{	
	BindTextures(shader);

	Renderer::Draw(m_VertexArray, m_IndexBuffer, shader);
	m_VertexArray.Unbind();	
}

void Mesh::DrawInstanced(Shader& shader, int instancedAmount)
{
	BindTextures(shader);

	Renderer::DrawInstanced(m_VertexArray, m_IndexBuffer, shader, instancedAmount);
	m_VertexArray.Unbind();
}

void Mesh::BindTextures(Shader& shader)
{
	std::vector<int> textureIndex = { 0, 0, 0, 0 };

	for (unsigned int i = 0; i < m_Textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		Texture::TextureType type = (*m_Textures[i]).GetTextureType();
		int texIndex = textureIndex[(int)type];

		shader.SetInt(GetUniformName(type) + std::to_string(texIndex), i);
		(*m_Textures[i]).Bind();
	}
}

std::string Mesh::GetUniformName(Texture::TextureType textureType)
{
	std::string string = "texture_";

	switch (textureType)
	{
	case Texture::TextureType::DIFFUSE:
		return string + "diffuse";
		break;
	case Texture::TextureType::SPECULAR:
		return string + "specular";
		break;
	case Texture::TextureType::NORMAL:
		return string + "normal";
		break;
	case Texture::TextureType::HEIGHT:
		return string + "height";
		break;
	}
}

