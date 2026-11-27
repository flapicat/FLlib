#include <glad/glad.h>
#include "Shader.h"

#include <fstream>
#include <sstream>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

#include "Log.h"

namespace FLlib
{
	static std::string ReadFile(const std::string& path)
	{
		std::ifstream file(path, std::ios::in | std::ios::binary);
		if (!file)
		{
			LOG_ERROR("Could not open shader file \"{}\"", path);
			return {};
		}
		std::stringstream stream;
		stream << file.rdbuf();
		return stream.str();
	}

	Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		uint32_t vertex = CompileStage(GL_VERTEX_SHADER, vertexSource);
		uint32_t fragment = CompileStage(GL_FRAGMENT_SHADER, fragmentSource);

		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, vertex);
		glAttachShader(m_RendererID, fragment);
		glLinkProgram(m_RendererID);

		int success = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
		if (!success)
		{
			int length = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> infoLog(length > 0 ? length : 1);
			glGetProgramInfoLog(m_RendererID, (GLsizei)infoLog.size(), nullptr, infoLog.data());
			LOG_ERROR("Shader program failed to link:\n{}", infoLog.data());
		}

		glDetachShader(m_RendererID, vertex);
		glDetachShader(m_RendererID, fragment);
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	std::shared_ptr<Shader> Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		return std::make_shared<Shader>(vertexSource, fragmentSource);
	}

	std::shared_ptr<Shader> Shader::CreateFromFiles(const std::string& vertexPath, const std::string& fragmentPath)
	{
		return std::make_shared<Shader>(ReadFile(vertexPath), ReadFile(fragmentPath));
	}

	uint32_t Shader::CompileStage(uint32_t type, const std::string& source)
	{
		uint32_t shader = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		int success = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			int length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> infoLog(length > 0 ? length : 1);
			glGetShaderInfoLog(shader, (GLsizei)infoLog.size(), nullptr, infoLog.data());
			LOG_ERROR("{} shader failed to compile:\n{}", type == GL_VERTEX_SHADER ? "Vertex" : "Fragment", infoLog.data());
		}
		return shader;
	}

	int Shader::GetUniformLocation(const std::string& name)
	{
		auto it = m_UniformLocationCache.find(name);
		if (it != m_UniformLocationCache.end())
			return it->second;

		int location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			LOG_WARN("Uniform \"{}\" does not exist in the shader or is unused", name);

		m_UniformLocationCache[name] = location;
		return location;
	}

	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetInt(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void Shader::SetIntArray(const std::string& name, const int* values, uint32_t count)
	{
		glUniform1iv(GetUniformLocation(name), count, values);
	}

	void Shader::SetFloat(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void Shader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		glUniform2f(GetUniformLocation(name), value.x, value.y);
	}

	void Shader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
	}

	void Shader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void Shader::SetMat3(const std::string& name, const glm::mat3& value)
	{
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}
}
