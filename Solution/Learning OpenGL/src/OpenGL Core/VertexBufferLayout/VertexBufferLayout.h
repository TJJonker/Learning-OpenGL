#pragma once
#include <vector>
#include "VertexBufferLayoutElement.h"

class VertexBufferLayout {
private: 
	std::vector<VertexBufferLayoutElement> m_Elements;
	unsigned int m_Stride;

public:
	VertexBufferLayout() : m_Stride(0) {}
	
	template<typename T>
	void Push(unsigned int count) {
		//std::runtime_error(false, "Unsupported data type for Push: ");
	}

	template<>
	void Push<unsigned int>(unsigned int count) {
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += VertexBufferLayoutElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<float>(unsigned int count) {
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += VertexBufferLayoutElement::GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count) {
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_FALSE });
		m_Stride += VertexBufferLayoutElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<VertexBufferLayoutElement> GetElements() const& { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};