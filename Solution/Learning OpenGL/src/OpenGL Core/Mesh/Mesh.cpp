#include "Mesh.h"
#include "../Renderer/Renderer.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices):
	m_VertexArray(), m_IndexBuffer(indices.data(), indices.size())
{
	// Create vertexBufferLayout
	VertexBuffer vertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex));

	VertexBufferLayout vertexBufferLayout;

	// Allocate space for certain memory
	vertexBufferLayout.Push<float>(3); // Position

	// Bind it to the vertexArray
	m_VertexArray.AddBuffer(vertexBuffer, vertexBufferLayout);
	m_VertexArray.Unbind();
}

void Mesh::Draw(Shader& shader)
{	
	Renderer::Draw(m_VertexArray, m_IndexBuffer, shader);
	m_VertexArray.Unbind();
	
}