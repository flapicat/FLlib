#include "Renderer.h"
#include <memory>
#include <cstring>
#include <cmath>

#include <glm/gtc/constants.hpp>

#include <glad/glad.h>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Log.h"
#include "Globals.h"

namespace FLlib {
	static const char* VertexShader = R"(
		#version 460 core
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec4 a_Color;

		uniform mat4 u_ViewProjection;

		out vec4 v_Color;

		void main()
		{
			v_Color = a_Color;
			gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
		}
	)";

	static const char* FragmentShader = R"(
		#version 460 core
		in vec4 v_Color;

		out vec4 o_Color;

		void main()
		{
			o_Color = v_Color;
		}
	)";

	static_assert(sizeof(Vertex) == sizeof(float) * (3 + 4), "Vertex must match the buffer layout");

	struct RendererData
	{
		static constexpr uint32_t MaxVertices = 100000;
		static constexpr uint32_t MaxIndices = 100000;

		std::shared_ptr<VertexArray> VertexArray;
		std::shared_ptr<VertexBuffer> VertexBuffer;
		std::shared_ptr<IndexBuffer> IndexBuffer;
		std::shared_ptr<Shader> Shader;

		Vertex Vertices[MaxVertices];
		uint32_t Indices[MaxIndices];
		uint32_t VertexCount = 0;
		uint32_t IndexCount = 0;

		glm::mat4 ViewProjection = glm::mat4(1.0f);
	};
	static std::unique_ptr<RendererData> s_Data;

	void Renderer::Init() {
		s_Data = std::make_unique<RendererData>();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		s_Data->VertexArray = VertexArray::Create();

		s_Data->VertexBuffer = VertexBuffer::Create(RendererData::MaxVertices * sizeof(Vertex));
		s_Data->VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
			});
		s_Data->VertexArray->SetVertexBuffer(s_Data->VertexBuffer);

		s_Data->IndexBuffer = IndexBuffer::Create(RendererData::MaxIndices);
		s_Data->VertexArray->SetIndexBuffer(s_Data->IndexBuffer);

		s_Data->Shader = Shader::Create(VertexShader, FragmentShader);
	}

	void Renderer::BeginRender() {
		s_Data->ViewProjection = Globals::Camera.GetViewProjection();
		StartBatch();
	}

	void Renderer::EndRender() {
		Flush();
	}

	void Renderer::RenderLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, float thickness)
	{
		const glm::vec3 delta = end - start;
		const float length = glm::length(delta);
		if (length <= 0.0f || thickness <= 0.0f)
			return;

		const glm::vec3 dir = delta / length;
		const glm::vec3 helper = std::abs(dir.y) < 0.99f ? glm::vec3(0.0f, 1.0f, 0.0f) : glm::vec3(1.0f, 0.0f, 0.0f);
		const glm::vec3 a = glm::normalize(glm::cross(helper, dir)) * (thickness * 0.5f);
		const glm::vec3 b = glm::normalize(glm::cross(dir, a)) * (thickness * 0.5f);

		Vertex vertices[] = {
			{start - a - b, color},
			{start + a - b, color},
			{start + a + b, color},
			{start - a + b, color},

			{end - a - b, color},
			{end + a - b, color},
			{end + a + b, color},
			{end - a + b, color}
		};

		uint32_t indices[] = {
			// Start
			0, 2, 1, 0, 3, 2,
			// End
			4, 5, 6, 4, 6, 7,
			// Sides
			0, 1, 5, 0, 5, 4,
			1, 2, 6, 1, 6, 5,
			2, 3, 7, 2, 7, 6,
			3, 0, 4, 3, 4, 7
		};

		RenderObject(vertices, 8, indices, 36);
	}

	void Renderer::RenderSphere(const glm::vec3& position, float radius, const glm::vec4& color)
	{
		constexpr uint32_t Stacks = 16;   // rings from top to bottom
		constexpr uint32_t Sectors = 32;  // slices around the Y axis
		constexpr uint32_t VertexCount = (Stacks + 1) * (Sectors + 1);
		constexpr uint32_t IndexCount = Sectors * (Stacks - 1) * 6;

		Vertex vertices[VertexCount];
		uint32_t indices[IndexCount];

		const float pi = glm::pi<float>();

		uint32_t v = 0;
		for (uint32_t i = 0; i <= Stacks; i++)
		{
			const float phi = pi * 0.5f - pi * (float)i / Stacks;  // +90 deg (top) to -90 deg (bottom)
			const float ringRadius = radius * cos(phi);
			const float y = radius * sin(phi);

			for (uint32_t j = 0; j <= Sectors; j++)
			{
				const float theta = 2.0f * pi * (float)j / Sectors;
				vertices[v++] = { position + glm::vec3(ringRadius * cos(theta), y, -ringRadius * sin(theta)), color };
			}
		}

		uint32_t n = 0;
		for (uint32_t i = 0; i < Stacks; i++)
		{
			uint32_t k1 = i * (Sectors + 1);  // current ring
			uint32_t k2 = k1 + Sectors + 1;   // next ring

			for (uint32_t j = 0; j < Sectors; j++, k1++, k2++)
			{
				// Top and bottom rings collapse to a point, so they only need one triangle per sector
				if (i != 0)
				{
					indices[n++] = k1;
					indices[n++] = k2;
					indices[n++] = k1 + 1;
				}
				if (i != Stacks - 1)
				{
					indices[n++] = k1 + 1;
					indices[n++] = k2;
					indices[n++] = k2 + 1;
				}
			}
		}

		RenderObject(vertices, VertexCount, indices, IndexCount);
	}
	
	void Renderer::RenderRect(const glm::vec3& position, const glm::vec3& size, const RectColor& color) {

		Vertex vertices[] = {
			// Back
			{{-0.5f, -0.5f, -0.5f}, color.Back},
			{{ 0.5f, -0.5f, -0.5f}, color.Back},
			{{ 0.5f,  0.5f, -0.5f}, color.Back},
			{{-0.5f,  0.5f, -0.5f}, color.Back},

			// Front
			{{-0.5f, -0.5f,  0.5f}, color.Front},
			{{ 0.5f, -0.5f,  0.5f}, color.Front},
			{{ 0.5f,  0.5f,  0.5f}, color.Front},
			{{-0.5f,  0.5f,  0.5f}, color.Front},

			// Left
			{{-0.5f, -0.5f, -0.5f}, color.Left},
			{{-0.5f,  0.5f, -0.5f}, color.Left},
			{{-0.5f,  0.5f,  0.5f}, color.Left},
			{{-0.5f, -0.5f,  0.5f}, color.Left},

			// Right
			{{ 0.5f, -0.5f, -0.5f}, color.Right},
			{{ 0.5f,  0.5f, -0.5f}, color.Right},
			{{ 0.5f,  0.5f,  0.5f}, color.Right},
			{{ 0.5f, -0.5f,  0.5f}, color.Right},

			// Bottom
			{{-0.5f, -0.5f, -0.5f}, color.Bottom},
			{{ 0.5f, -0.5f, -0.5f}, color.Bottom},
			{{ 0.5f, -0.5f,  0.5f}, color.Bottom},
			{{-0.5f, -0.5f,  0.5f}, color.Bottom},

			// Top
			{{-0.5f,  0.5f, -0.5f}, color.Top},
			{{ 0.5f,  0.5f, -0.5f}, color.Top},
			{{ 0.5f,  0.5f,  0.5f}, color.Top},
			{{-0.5f,  0.5f,  0.5f}, color.Top}
		};

		uint32_t indices[] = {
			// Back
			0, 1, 2, 2, 3, 0,
			// Front
			4, 6, 5, 6, 4, 7,
			// Left
			8, 9, 10, 10, 11, 8,
			// Right
			12, 14, 13, 14, 12, 15,
			// Bottom
			16, 17, 18, 18, 19, 16,
			// Top
			20, 22, 21, 22, 20, 23
		};

		// Scale the unit cube to `size` and move it to `position` (its center)
		for (Vertex& vertex : vertices)
			vertex.Position = position + vertex.Position * size;

		RenderObject(vertices, 24, indices, 36);
	}

	void Renderer::RenderObject(const Vertex* vertices, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount)
	{
		if (vertexCount > RendererData::MaxVertices || indexCount > RendererData::MaxIndices)
		{
			LOG_ERROR("Renderer::PushObject: object too large for a batch ({} vertices, {} indices)", vertexCount, indexCount);
			return;
		}

		if (s_Data->VertexCount + vertexCount > RendererData::MaxVertices ||
			s_Data->IndexCount + indexCount > RendererData::MaxIndices)
			NextBatch();

		const uint32_t baseVertex = s_Data->VertexCount;

		std::memcpy(&s_Data->Vertices[baseVertex], vertices, vertexCount * sizeof(Vertex));

		for (uint32_t i = 0; i < indexCount; i++)
			s_Data->Indices[s_Data->IndexCount + i] = baseVertex + indices[i];

		s_Data->VertexCount += vertexCount;
		s_Data->IndexCount += indexCount;
	}

	void Renderer::StartBatch()
	{
		s_Data->VertexCount = 0;
		s_Data->IndexCount = 0;
	}

	void Renderer::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer::Flush()
	{
		if (s_Data->IndexCount == 0)
			return;

		s_Data->VertexBuffer->SetData(s_Data->Vertices, s_Data->VertexCount * sizeof(Vertex));
		s_Data->IndexBuffer->SetData(s_Data->Indices, s_Data->IndexCount);

		s_Data->Shader->Bind();
		s_Data->Shader->SetMat4("u_ViewProjection", s_Data->ViewProjection);

		s_Data->VertexArray->Bind();
		glDrawElements(GL_TRIANGLES, s_Data->IndexCount, GL_UNSIGNED_INT, nullptr);
	}
}
