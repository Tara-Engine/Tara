#pragma once
#include "Tara/Renderer/Shader.h"

namespace Tara {

	class OpenGLShader : public Shader {
	public:
		//OpenGLShader(const std::string& name, Shader::SourceType type,const std::string& vertexSrc,const std::string& fragmentSrc);
		OpenGLShader(const std::string& name, Shader::SourceType type, std::unordered_map<TargetStage, std::string> sources);
		

		~OpenGLShader();

		virtual void ImplBind(int, int) const override;
		
		//uniform checking
		virtual bool ValidUniform(const std::string& name) override;

		//uniform sending
		virtual void Send(const std::string& name, int value) override;
		virtual void Send(const std::string& name, int* value, int count) override;
		virtual void Send(const std::string& name, float value) override;
		virtual void Send(const std::string& name, const glm::vec2& value) override;
		virtual void Send(const std::string& name, const glm::vec3& value) override;
		virtual void Send(const std::string& name, const glm::vec4& value) override;
		virtual void Send(const std::string& name, const glm::mat3& value) override;
		virtual void Send(const std::string& name, const glm::mat4& value) override;

	public:
		static uint32_t DatatypeToGLenum(Shader::Datatype type);

	private:
		static uint32_t TargetStageToGLenum(Shader::TargetStage stage);

		void Build(const std::unordered_map<uint32_t, std::string>& sources);
		std::string ReadShaderTextFile(const std::string& filename);

		int32_t GetUniformLocation(const std::string& name);

	private:
		uint32_t m_RendererID;
		std::unordered_map<std::string, int32_t> m_UniformCache;

	};

}