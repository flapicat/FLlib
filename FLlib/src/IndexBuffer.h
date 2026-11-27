#pragma once
#include <cstdint>
#include <memory>

namespace FLlib
{
	class IndexBuffer
	{
	public:
		IndexBuffer(uint32_t count);
		IndexBuffer(const uint32_t* indices, uint32_t count);
		~IndexBuffer();

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;

		static std::shared_ptr<IndexBuffer> Create(uint32_t count);
		static std::shared_ptr<IndexBuffer> Create(const uint32_t* indices, uint32_t count);

		void Bind() const;
		void Unbind() const;

		// Uploads `count` indices to the start of the buffer (dynamic buffers only)
		void SetData(const uint32_t* indices, uint32_t count);

		uint32_t GetCount() const { return m_Count; }
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Count = 0;
	};
}
