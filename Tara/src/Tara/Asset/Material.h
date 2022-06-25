#pragma once
#include "Tara/Asset/Asset.h"
#include "Tara/Renderer/Shader.h"
#include "Tara/Renderer/Texture.h"
#include "Tara/Renderer/Uniform.h"
#include "Tara/Renderer/MaterialBase.h"

namespace Tara {
	REFTYPE(Material);

	class Material : public MaterialBase, public Asset {
	public:

		Material(MaterialType type, const std::string& source, Shader::SourceType sourceType, const std::string& name);
		
		~Material() = default;

		static MaterialRef Create(MaterialType type, const std::string& source, Shader::SourceType sourceType, const std::string& name);

		/// <summary>
		/// Use this material. Binds the underlying shader and sets up the uniforms.
		/// </summary>
		virtual void Use() override;

		/// <summary>
		/// Get the material types
		/// </summary>
		/// <returns></returns>
		inline virtual MaterialType GetType() override { return m_Type; };

		/// <summary>
		/// Get the shader being used.
		/// </summary>
		/// <returns></returns>
		inline virtual const ShaderRef& GetShader() override { return m_Shader; };

		/// <summary>
		/// Get a Parameter list
		/// </summary>
		/// <returns></returns>
		virtual std::vector<std::pair<std::string, MaterialParameterType>> GetParameterList() override;

		/// <summary>
		/// Get the type of the Parameter. If the name is not a valid Parameter, then a warning will be printed, and Float1 returned.
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		virtual MaterialParameterType GetParameterType(const std::string& name) override;

		/// <summary>
		/// Check if a Parameter name is a valid Parameter
		/// </summary>
		/// <param name="name">the Parameter name</param>
		/// <returns></returns>
		virtual bool GetParameterValid(const std::string& name) override;

		/// <summary>
		/// Get the current value of a Parameter
		/// </summary>
		/// <param name="name">the name of the paramamter</param>
		/// <returns>pair of the type and the value</returns>
		virtual std::pair<MaterialParameterType, MaterialParameter> GetParameterValue(const std::string& name) override;

		/// <summary>
		/// Base setting of a Parameter
		/// </summary>
		/// <param name="name">the Parameter name</param>
		/// <param name="value">the Parameter value</param>
		virtual void SetParameter(const std::string& name, MaterialParameter value) override;

	private:
		/// <summary>
		/// Run the source through the preprocessor, to see what should be included
		/// </summary>
		/// <param name="source"></param>
		/// <returns></returns>
		std::string PreprocessSource(const std::string& source);

		/// <summary>
		/// Load the Material's shader from a direct string source
		/// </summary>
		/// <param name="source"></param>
		void ShaderFromString(const std::string& source);

		/// <summary>
		/// Load the Material's Parameters from a direct string source
		/// </summary>
		/// <param name="source"></param>
		void ParametersFromString(const std::string& source);

		/// <summary>
		/// Read a text file in from source
		/// </summary>
		/// <param name="filename">the file name</param>
		/// <returns>the returned contents</returns>
		static std::string ReadSourceTextFile(const std::string& filename);

	private:
		//the actual shader
		ShaderRef m_Shader;
		//map of Parameter names to the values
		std::unordered_map<std::string, std::pair<MaterialParameterType, MaterialParameter>> m_Parameters;
		const MaterialType m_Type;
	private:
		static std::unordered_map<MaterialType, std::string> SourcePartsVertex;
		static std::unordered_map<MaterialType, std::string> SourcePartsFragmentBegin;
		static std::unordered_map<MaterialType, std::string> SourcePartsFragmentEnd;
		static std::unordered_map<std::string, std::string> SourceIncludes;
	};
}