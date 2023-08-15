#pragma once
#include "../VertexArray/VertexArray.h"
#include "../IndexBuffer/IndexBuffer.h"
#include "../Shader/Shader.h"


class Renderer
{
public:
	void Clear() const;
	static void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader);
};

