#include <glad/glad.h>
#include "VertexArray.h"

#include "Log.h"

namespace FLlib
{
	static GLenum ShaderDataTypeToGLType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Bool:		return GL_BOOL;
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:		return GL_FLOAT;
		default:						return GL_NONE;
		}
	}

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		return std::make_shared<VertexArray>();
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		const BufferLayout& layout = vertexBuffer->GetLayout();
		if (layout.GetElements().empty())
		{
			LOG_ERROR("VertexArray::AddVertexBuffer: the vertex buffer has no layout, call SetLayout first");
			return;
		}

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		for (const BufferElement& element : layout)
		{
			switch (element.Type)
			{
			case ShaderDataType::Bool:
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			{
				glEnableVertexAttribArray(m_AttribIndex);
				glVertexAttribIPointer(m_AttribIndex,
					GetShaderDataTypeCount(element.Type),
					ShaderDataTypeToGLType(element.Type),
					layout.GetStride(),
					(const void*)(uintptr_t)element.Offset);
				m_AttribIndex++;
				break;
			}
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(m_AttribIndex);
				glVertexAttribPointer(m_AttribIndex,
					GetShaderDataTypeCount(element.Type),
					ShaderDataTypeToGLType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)(uintptr_t)element.Offset);
				m_AttribIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint32_t columns = element.Type == ShaderDataType::Mat3 ? 3 : 4;
				for (uint32_t i = 0; i < columns; i++)
				{
					glEnableVertexAttribArray(m_AttribIndex);
					glVertexAttribPointer(m_AttribIndex,
						columns,
						GL_FLOAT,
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(uintptr_t)(element.Offset + sizeof(float) * columns * i));
					m_AttribIndex++;
				}
				break;
			}
			default:
				LOG_ERROR("VertexArray::AddVertexBuffer: unknown type of attribute \"{}\"", element.Name);
				break;
			}
		}

		m_VertexBuffer = vertexBuffer;
	}

	void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}
}
