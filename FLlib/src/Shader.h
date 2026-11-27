#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace FLlib
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		static std::shared_ptr<Shader> Create(const std::string& vertexSource, const std::string& fragmentSource);
		static std::shared_ptr<Shader> CreateFromFiles(const std::string& vertexPath, const std::string& fragmentPath);

		void Bind() const;
		void Unbind() const;

		void SetInt(const std::string& name, int value);
		void SetIntArray(const std::string& name, const int* values, uint32_t count);
		void SetFloat(const std::string& name, float value);
		void SetFloat2(const std::string& name, const glm::vec2& value);
		void SetFloat3(const std::string& name, const glm::vec3& value);
		void SetFloat4(const std::string& name, const glm::vec4& value);
		void SetMat3(const std::string& name, const glm::mat3& value);
		void SetMat4(const std::string& name, const glm::mat4& value);
	private:
		uint32_t CompileStage(uint32_t type, const std::string& source);
		int GetUniformLocation(const std::string& name);
	private:
		uint32_t m_RendererID = 0;
		std::unordered_map<std::string, int> m_UniformLocationCache;
	};
}
