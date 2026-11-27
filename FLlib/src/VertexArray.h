#pragma once
#include <memory>
#include <vector>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace FLlib
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		static std::shared_ptr<VertexArray> Create();

		void Bind() const;
		void Unbind() const;

		void SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

		const std::shared_ptr<VertexBuffer>& GetVertexBuffer() const { return m_VertexBuffer; }
		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_AttribIndex = 0;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};
}
