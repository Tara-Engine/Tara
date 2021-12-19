#pragma once
#include "glm/glm.hpp"
#include "Tara/Math/Types.h"
#include "Tara/Asset/Asset.h"

namespace Tara {
	/// <summary>
	/// Shader class for storing and making shaders
	/// </summary>
	class Shader : public Asset{
		/// <summary>
		/// Shader reference
		/// </summary>
		using ShaderRef = std::shared_ptr<Shader>;
	public:
		Shader(const std::string name)
			: Asset(name) {}

		virtual ~Shader() {}
		/// <summary>
		/// Bind the current shader
		/// </summary>
		virtual void Bind() const = 0;
		/// <summary>
		/// Unbind the current shader (not nesecary to use)
		/// </summary>
		virtual void Unbind() const = 0;

		/// <summary>
		/// Check if a uniform is valid or not
		/// </summary>
		/// <param name="name">the uniform name</param>
		/// <returns>true if uniform is valid, false otherwise</returns>
		virtual bool ValidUniform(const std::string& name) = 0;

		/// <summary>
		/// Sends a value to a uniform in the Shader
		/// </summary>
		/// <param name="name">Name of the uniform</param>
		/// <param name="value">value to send</param>
		virtual void Send(const std::string& name, int value) = 0;

		/// <summary>
		/// Send a value to a uniform in the Shader
		/// </summary>
		/// <param name="name">the name of the uniform</param>
		/// <param name="value">value to send (array of ints)</param>
		/// <param name="count">number of ints being sent</param>
		virtual void Send(const std::string& name, int* value, int count) = 0;

		/// <summary>
		/// Sends a value to a uniform in the Shader
		/// </summary>
		/// <param name="name">Name of the uniform</param>
		/// <param name="value">value to send</param>
		virtual void Send(const std::string& name, float value) = 0;
		/// <summary>
		/// Sends a value to a uniform in the Shader
		/// </summary>
		/// <param name="name">Name of the uniform</param>
		/// <param name="value">value to send</param>
		virtual void Send(const std::string& name, const glm::vec2& value) = 0;
		/// <summary>
		/// Sends a value to a uniform in the Shader
		/// </summary>
		/// <param name="name">Name of the uniform</param>
		/// <param name="value">value to send</param>
		virtual void Send(const std::string& name, const glm::vec3& value) = 0;
		/// <summary>
		/// Sends a value to a uniform in the Shader
		/// </summary>
		/// <param name="name">Name of the uniform</param>
		/// <param name="value">value to send</param>
		inline virtual void Send(const std::string& name, const Vector& value) { Send(name, (glm::vec3)value); };
		/// <summary>
		/// Sends a value to a uniform in the Shader
		/// </summary>
		/// <param name="name">Name of the uniform</param>
		/// <param name="value">value to send</param>
		virtual void Send(const std::string& name, const glm::vec4& value) = 0;
		/// <summary>
		/// Sends a value to a uniform in the Shader
		/// </summary>
		/// <param name="name">Name of the uniform</param>
		/// <param name="value">value to send</param>
		virtual void Send(const std::string& name, const glm::mat3& value) = 0;
		/// <summary>
		/// Sends a value to a uniform in the Shader
		/// </summary>
		/// <param name="name">Name of the uniform</param>
		/// <param name="value">value to send</param>
		virtual void Send(const std::string& name, const glm::mat4& value) = 0;


	public:

		/// <summary>
		/// Source Type of a shader. Possible values are: 
		/// Strings, TextFiles, BinaryFiles
		/// Binary Files refers to SPIR-V shaders
		/// </summary>
		enum class SourceType : uint8_t {
			Strings=0, TextFiles=1, BinaryFiles=2
		};
		
		/// <summary>
		/// Data types for shader uniforms and vertex buffer layouts
		/// </summary>
		enum class Datatype : uint8_t {
			None = 0,
			Float, Float2, Float3, Float4,
			Int, Int2, Int3, Int4,
			Mat3, Mat4,
			Bool
		};

		/// <summary>
		/// Particular source file target stage in the graphics pipeline
		/// </summary>
		enum class TargetStage : uint8_t {
			Compute, Vertex, TessellationControl, TessellationEvaluation, Geometry, Pixel
		};

	public:
		/// <summary>
		/// Get the size of a Shader::Datatype
		/// </summary>
		/// <param name="type">the Datatype to scan</param>
		/// <returns>size in bytes</returns>
		static uint32_t GetDatatypeSize(Shader::Datatype type);
		/// <summary>
		/// Get the count (in base elements) of the Shader::Datatype
		/// </summary>
		/// <param name="type">the Datatype to scan</param>
		/// <returns>count in elements</returns>
		static uint32_t GetDatatypeCount(Shader::Datatype type);

		/// <summary>
		/// Create a new shader
		/// </summary>
		/// <param name="type">the input type. 
		/// If Shader::Datatype::Strings, the remaining paramaters are treated as raw code
		/// If Shader::Datatype::TextFiles, the remaining paramaters are treated as filepaths to text code files
		/// If Shader::Datatype::BinaryFiles, the remaining paramaters are treated as filepaths to binary SPIR-V code files
		/// </param>
		/// <param name="vertexSrc">Raw code / filepath for the vertex stage</param>
		/// <param name="fragmentSrc">Raw code / filepath for the fragment stage</param>
		/// <returns></returns>
		static ShaderRef Create(
			const std::string& name,
			Shader::SourceType type, 
			const std::string& vertexSrc, 
			const std::string& fragmentSrc
		);

		/// <summary>
		/// Create a new shader
		/// </summary>
		/// <param name="name">the shader name</param>
		/// <param name="type">the shader type</param>
		/// <param name="sources">the shader source map</param>
		/// <returns></returns>
		static ShaderRef Create(
			const std::string& name,
			Shader::SourceType type,
			std::unordered_map<TargetStage, std::string> sources
		);

	};
	/// <summary>
	/// Reference to a Shader class for storing and making shaders
	/// </summary>
	using ShaderRef = std::shared_ptr<Shader>;
}