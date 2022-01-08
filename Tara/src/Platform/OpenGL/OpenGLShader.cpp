#include "tarapch.h"
#include "OpenGLShader.h"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include <array>
#include <fstream>

namespace Tara{

	/*
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
	*/

	OpenGLShader::OpenGLShader(const std::string& name, Shader::SourceType type, std::unordered_map<TargetStage, std::string> sources)
		: Shader(name), m_RendererID(0)
	{
		switch (type) {
		case Shader::SourceType::Strings: {
			std::unordered_map<GLenum, std::string> sourcesParsed;
			for (auto kv : sources) {
				sourcesParsed[TargetStageToGLenum(kv.first)] = kv.second;
			}
			Build(sourcesParsed);
			break;
		}
		case SourceType::TextFiles: {
			std::unordered_map<GLenum, std::string> sourcesParsed;
			for (auto kv : sources) {
				sourcesParsed[TargetStageToGLenum(kv.first)] = ReadShaderTextFile(kv.second);
			}
			Build(sourcesParsed);
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

	void OpenGLShader::ImplBind(int, int) const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::ImplUnbind() const
	{
		glUseProgram(0);
	}

	bool OpenGLShader::ValidUniform(const std::string& name)
	{
		auto cache = m_UniformCache.find(name);
		if (cache != m_UniformCache.end()) {
			return true; //true, and cached
		}
		else {
			int32_t loc = glGetUniformLocation(m_RendererID, name.c_str());
			if (loc == -1) {
				return false; //not cached, and not valid
			}
			else {
				m_UniformCache.insert_or_assign(name, loc);
				return true; //not cached previously (now it is!) and valid
			}
		}
	}
	/*
	void OpenGLShader::Send(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::Send(const std::string& name, int* value, int count)
	{
		glUniform1iv(GetUniformLocation(name), count, value);
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
	*/


	void OpenGLShader::ImplSend(const std::string& name, const Uniform& uniform)
	{
		auto loc = GetUniformLocation(name);
		switch (uniform.DataType) {
		case UniformType::Float1: glUniform1f(loc, uniform.Data.Float1); break;
		case UniformType::Float2: glUniform2f(loc, uniform.Data.Float2.x, uniform.Data.Float2.y); break;
		case UniformType::Float3: glUniform3f(loc, uniform.Data.Float3.x, uniform.Data.Float3.y, uniform.Data.Float3.z); break;
		case UniformType::Float4: glUniform4f(loc, uniform.Data.Float4.x, uniform.Data.Float4.y, uniform.Data.Float4.z, uniform.Data.Float4.w); break;

		case UniformType::Int1: glUniform1i(loc, uniform.Data.Int1); break;
		case UniformType::Int2: glUniform2i(loc, uniform.Data.Int2.x, uniform.Data.Int2.y); break;
		case UniformType::Int3: glUniform3i(loc, uniform.Data.Int3.x, uniform.Data.Int3.y, uniform.Data.Int3.z); break;
		case UniformType::Int4: glUniform4i(loc, uniform.Data.Int4.x, uniform.Data.Int4.y, uniform.Data.Int4.z, uniform.Data.Int4.w); break;

		case UniformType::UInt1: glUniform1ui(loc, uniform.Data.UInt1); break;
		case UniformType::UInt2: glUniform2ui(loc, uniform.Data.UInt2.x, uniform.Data.UInt2.y); break;
		case UniformType::UInt3: glUniform3ui(loc, uniform.Data.UInt3.x, uniform.Data.UInt3.y, uniform.Data.UInt3.z); break;
		case UniformType::UInt4: glUniform4ui(loc, uniform.Data.UInt4.x, uniform.Data.UInt4.y, uniform.Data.UInt4.z, uniform.Data.UInt4.w); break;

		case UniformType::Float1Vector: glUniform1fv(loc, uniform.Data.FloatVector.Count, uniform.Data.FloatVector.Data); break;
		case UniformType::Float2Vector: glUniform2fv(loc, uniform.Data.FloatVector.Count, uniform.Data.FloatVector.Data); break;
		case UniformType::Float3Vector: glUniform3fv(loc, uniform.Data.FloatVector.Count, uniform.Data.FloatVector.Data); break;
		case UniformType::Float4Vector: glUniform4fv(loc, uniform.Data.FloatVector.Count, uniform.Data.FloatVector.Data); break;

		case UniformType::Int1Vector: glUniform1iv(loc, uniform.Data.IntVector.Count, uniform.Data.IntVector.Data); break;
		case UniformType::Int2Vector: glUniform2iv(loc, uniform.Data.IntVector.Count, uniform.Data.IntVector.Data); break;
		case UniformType::Int3Vector: glUniform3iv(loc, uniform.Data.IntVector.Count, uniform.Data.IntVector.Data); break;
		case UniformType::Int4Vector: glUniform4iv(loc, uniform.Data.IntVector.Count, uniform.Data.IntVector.Data); break;

		case UniformType::UInt1Vector: glUniform1uiv(loc, uniform.Data.UIntVector.Count, uniform.Data.UIntVector.Data); break;
		case UniformType::UInt2Vector: glUniform2uiv(loc, uniform.Data.UIntVector.Count, uniform.Data.UIntVector.Data); break;
		case UniformType::UInt3Vector: glUniform3uiv(loc, uniform.Data.UIntVector.Count, uniform.Data.UIntVector.Data); break;
		case UniformType::UInt4Vector: glUniform4uiv(loc, uniform.Data.UIntVector.Count, uniform.Data.UIntVector.Data); break;

		case UniformType::Matrix2Vector: glUniformMatrix2fv(loc, uniform.Data.FloatVector.Count, GL_FALSE, uniform.Data.FloatVector.Data); break;
		case UniformType::Matrix3Vector: glUniformMatrix3fv(loc, uniform.Data.FloatVector.Count, GL_FALSE, uniform.Data.FloatVector.Data); break;
		case UniformType::Matrix4Vector: glUniformMatrix4fv(loc, uniform.Data.FloatVector.Count, GL_FALSE, uniform.Data.FloatVector.Data); break;
		
		case UniformType::Matrix2x3Vector: glUniformMatrix2x3fv(loc, uniform.Data.FloatVector.Count, GL_FALSE, uniform.Data.FloatVector.Data); break;
		case UniformType::Matrix3x2Vector: glUniformMatrix3x2fv(loc, uniform.Data.FloatVector.Count, GL_FALSE, uniform.Data.FloatVector.Data); break;
		
		case UniformType::Matrix2x4Vector: glUniformMatrix2x4fv(loc, uniform.Data.FloatVector.Count, GL_FALSE, uniform.Data.FloatVector.Data); break;
		case UniformType::Matrix4x2Vector: glUniformMatrix4x2fv(loc, uniform.Data.FloatVector.Count, GL_FALSE, uniform.Data.FloatVector.Data); break;
		
		case UniformType::Matrix3x4Vector: glUniformMatrix3x4fv(loc, uniform.Data.FloatVector.Count, GL_FALSE, uniform.Data.FloatVector.Data); break;
		case UniformType::Matrix4x3Vector: glUniformMatrix4x3fv(loc, uniform.Data.FloatVector.Count, GL_FALSE, uniform.Data.FloatVector.Data); break;

		}
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

	uint32_t OpenGLShader::TargetStageToGLenum(Shader::TargetStage stage)
	{
		switch (stage) {
		case Shader::TargetStage::Compute:					return GL_COMPUTE_SHADER;
		case Shader::TargetStage::Vertex:					return GL_VERTEX_SHADER;
		case Shader::TargetStage::TessellationControl:		return GL_TESS_CONTROL_SHADER;
		case Shader::TargetStage::TessellationEvaluation:	return GL_TESS_EVALUATION_SHADER;
		case Shader::TargetStage::Geometry:					return GL_GEOMETRY_SHADER;
		case Shader::TargetStage::Pixel:					return GL_FRAGMENT_SHADER;
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
				LOG_S(ERROR) << "Shader Code: [" << std::endl << source << std::endl << "]";

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
		LOG_S(INFO) << "Shader created";
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
			ABORT_F("Error! Attempted to open an invalid shader file: %s", filename.c_str());
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
			DCHECK_F((loc != -1), "Uniform [%s] does not exist!", name.c_str());
			m_UniformCache.insert_or_assign(name, loc);
			return loc;
		}
	}


}
