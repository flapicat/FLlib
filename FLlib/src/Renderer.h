#pragma once
#include <cstdint>

#include <glm/glm.hpp>

#include "Globals.h"

namespace FLlib {									  
	
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	struct RectColor {
		glm::vec4 Top    = Color::White;
		glm::vec4 Bottom = Color::White;
		glm::vec4 Left   = Color::White;
		glm::vec4 Right  = Color::White;
		glm::vec4 Back   = Color::White;
		glm::vec4 Front  = Color::White;

		void SetAllSides(const glm::vec4& color) {
			Top    = color;
			Bottom = color;
			Left   = color;
			Right  = color;
			Back   = color;
			Front  = color;
		}
	};

	class Renderer
	{
	public:
		static void Init();
		static void BeginRender();
		static void EndRender();

		static void RenderLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color = Color::White, float thickness = 0.01f);
		static void RenderSphere(const glm::vec3& position, float radius, const glm::vec4& color = Color::White);
		static void RenderRect(const glm::vec3& position, const glm::vec3& size, const RectColor& color = RectColor());
		static void RenderObject(const Vertex* vertices, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount);
	private:
		static void StartBatch();
		static void NextBatch();
		static void Flush();
	};
}
