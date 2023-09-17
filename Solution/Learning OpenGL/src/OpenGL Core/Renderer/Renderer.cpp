#include "Renderer.h"

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader)
{
	shader.Bind();
	vertexArray.Bind();
	indexBuffer.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawInstanced(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader, int instancedAmount)
{
	shader.Bind();
	vertexArray.Bind();
	indexBuffer.Bind();

	GLCall(glDrawElementsInstanced(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr, instancedAmount));
}
