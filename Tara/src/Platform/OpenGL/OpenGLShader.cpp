#include "tarapch.h"
#include "OpenGLShader.h"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include <array>
#include <fstream>

namespace Tara{

	OpenGLShader::OpenGLShader(
		const std::string& name,
		Shader::SourceType type, 
		const std::string& vertexSrc, 
		const std::string& fragmentSrc
	)
		: Shader(name), m_RendererID(0)
	{
		switch (type) {
		case Shader::SourceType::Strings: {
			std::unordered_map<GLenum, std::string> sources;
			sources[GL_VERTEX_SHADER] = vertexSrc;
			sources[GL_FRAGMENT_SHADER] = fragmentSrc;
			Build(sources);
			break;
		}
		case SourceType::TextFiles: {
			std::unordered_map<GLenum, std::string> sources;
			sources[GL_VERTEX_SHADER] = ReadShaderTextFile(vertexSrc);
			sources[GL_FRAGMENT_SHADER] = ReadShaderTextFile(fragmentSrc);
			Build(sources);
			break;
		}
		case SourceType::BinaryFiles: {
			ABORT_F("Binary SPIR-V shader files not yet supported!");
			break;
		}
		}
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::Send(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::Send(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void OpenGLShader::Send(const std::string& name, const glm::vec2& value)
	{
		glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(value));
	}

	void OpenGLShader::Send(const std::string& name, const glm::vec3& value)
	{
		glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
	}

	void OpenGLShader::Send(const std::string& name, const glm::vec4& value)
	{
		glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(value));
	}

	void OpenGLShader::Send(const std::string& name, const glm::mat3& value)
	{
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::Send(const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}



	uint32_t OpenGLShader::DatatypeToGLenum(Shader::Datatype type)
	{
		switch (type)
		{
		case Shader::Datatype::None:	return 0;
		case Shader::Datatype::Float:	return GL_FLOAT;
		case Shader::Datatype::Float2:	return GL_FLOAT;
		case Shader::Datatype::Float3:	return GL_FLOAT;
		case Shader::Datatype::Float4:	return GL_FLOAT;
		case Shader::Datatype::Mat3:	return GL_FLOAT;
		case Shader::Datatype::Mat4:	return GL_FLOAT;
		case Shader::Datatype::Int:		return GL_INT;
		case Shader::Datatype::Int2:	return GL_INT;
		case Shader::Datatype::Int3:	return GL_INT;
		case Shader::Datatype::Int4:	return GL_INT;
		case Shader::Datatype::Bool:	return GL_BOOL;
		}
		return 0;
	}

	void OpenGLShader::Build(const std::unordered_map<GLenum, std::string>& sources)
	{
		DCHECK_F(sources.size() < 6, "Shaders cannot have more than 5 sources!");
		std::array<uint32_t, 5> shader_IDs;
		uint8_t index = 0;
		
		uint32_t program = glCreateProgram();

		for (auto& kv : sources) {
			GLenum type = kv.first;
			const std::string& source = kv.second;

			uint32_t shader = glCreateShader(type);
			const char* srcCstr = source.c_str();
			glShaderSource(shader, 1, &srcCstr, 0);
			glCompileShader(shader);
			
			//error checking
			int32_t isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE) {
				int32_t maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<char> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
				LOG_S(ERROR) << "Shader Compilation Error [" << (uint32_t)index << "]: " << infoLog.data();

				//delete unused shader
				for (uint8_t s = 0; s < index; s++) {
					glDeleteShader(shader_IDs[s]);
				}
				glDeleteProgram(program);
				return;
			}
			glAttachShader(program, shader);
			shader_IDs[index++] = shader;
		}

		//now that all shader parts are compiled and attached
		//link the program

		glLinkProgram(program);

		//error checking!
		int32_t isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE) {
			int32_t maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			LOG_S(ERROR) << "Shader Link Error: " << infoLog.data();

			for (uint8_t s = 0; s < index; s++) {
				glDeleteShader(shader_IDs[s]);
			}
			glDeleteProgram(program);
			return;
		}

		//detatch and delete shaders
		for (uint8_t s = 0; s < index; s++) {
			glDetachShader(program, shader_IDs[s]);
			glDeleteShader(shader_IDs[s]);
		}

		m_RendererID = program;
	}

	std::string OpenGLShader::ReadShaderTextFile(const std::string& filename)
	{
		std::string result;
		std::ifstream in(filename, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end); //find end of file
			result.resize(in.tellg()); //resize string
			in.seekg(0, std::ios::beg); //return to begin
			in.read(&result[0], result.size()); //read file
			in.close();
		}
		else {
			ABORT_F("Error! Attempted to open an invalid shader file: %s", filename);
		}
		return result;
	}

	int32_t OpenGLShader::GetUniformLocation(const std::string& name)
	{
		auto cache = m_UniformCache.find(name);
		if (cache != m_UniformCache.end()) {
			return cache->second;
		}
		else {
			int32_t loc = glGetUniformLocation(m_RendererID, name.c_str());
			DCHECK_F((loc != -1), "Uniform [%s] does not exist!", name);
			m_UniformCache.insert_or_assign(name, loc);
			return loc;
		}
	}


}
