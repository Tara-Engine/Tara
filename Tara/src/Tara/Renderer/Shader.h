#pragma once
#include "Tara/Renderer/Renderer.h"
#include "glm/glm.hpp"

namespace Tara {

	class Shader {
		using ShaderRef = std::shared_ptr<Shader>;
	public:
		virtual ~Shader() {}
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		//uniform sending
		virtual void Send(const std::string& name, int value) = 0;
		virtual void Send(const std::string& name, float value) = 0;
		virtual void Send(const std::string& name, const glm::vec2& value) = 0;
		virtual void Send(const std::string& name, const glm::vec3& value) = 0;
		virtual void Send(const std::string& name, const glm::vec4& value) = 0;
		virtual void Send(const std::string& name, const glm::mat3& value) = 0;
		virtual void Send(const std::string& name, const glm::mat4& value) = 0;


	public:

		enum class SourceType : uint8_t {
			Strings=0, TextFiles=1, BinaryFiles=2
		};
		
		enum class Datatype : uint8_t {
			None = 0,
			Float, Float2, Float3, Float4,
			Int, Int2, Int3, Int4,
			Mat3, Mat4,
			Bool
		};

	public:

		static uint32_t GetDatatypeSize(Shader::Datatype type);
		static uint32_t GetDatatypeCount(Shader::Datatype type);

		static ShaderRef Create(
			Shader::SourceType type, 
			const std::string& vertexSrc, 
			const std::string& fragmentSrc
		);
		/*TODO: add overloaded Create functions for shaders with other programmed steps:
			? compute (1 input string) (compute)
			+ geometry (3 input string) (vertex, geom, frag), 
			+ Tesselation(4 input strings) (vertex, tess ctrl, tess eval, frag), or
			+ both (5 input strings) (vertex, tess ctrl, tess eval, geom, frag)
		*/

	};
	using ShaderRef = std::shared_ptr<Shader>;
}