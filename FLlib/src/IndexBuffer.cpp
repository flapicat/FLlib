#include <glad/glad.h>
#include "IndexBuffer.h"

namespace FLlib
{
	IndexBuffer::IndexBuffer(uint32_t count)
		:m_Count(count)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);
	}

	IndexBuffer::IndexBuffer(const uint32_t* indices, uint32_t count)
		:m_Count(count)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t count)
	{
		return std::make_shared<IndexBuffer>(count);
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, uint32_t count)
	{
		return std::make_shared<IndexBuffer>(indices, count);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::SetData(const uint32_t* indices, uint32_t count)
	{
		// Bound to GL_ARRAY_BUFFER so the upload doesn't change whichever VAO is currently bound
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(uint32_t), indices);
	}
}
