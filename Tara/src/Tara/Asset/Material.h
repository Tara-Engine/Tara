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

		Material(const std::string& source, Shader::SourceType sourceType, const std::string& name);
		
		~Material();

		static MaterialRef Create(const std::string& source, Shader::SourceType sourceType, const std::string& name);

		/// <summary>
		/// Use this material. Binds the underlying shader and sets up the uniforms.
		/// </summary>
		virtual void Use() override;

		/// <summary>
		/// Get the shader being used.
		/// </summary>
		/// <returns></returns>
		inline virtual const ShaderRef& GetShader() override { return m_Shader; };

		/// <summary>
		/// Get a paramater list
		/// </summary>
		/// <returns></returns>
		virtual std::vector<std::pair<std::string, MaterialParamaterType>> GetParamaterList() override;

		/// <summary>
		/// Get the type of the paramater. If the name is not a valid paramater, then a warning will be printed, and Float1 returned.
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		virtual MaterialParamaterType GetParamaterType(const std::string& name) override;

		/// <summary>
		/// Check if a paramater name is a valid paramater
		/// </summary>
		/// <param name="name">the paramater name</param>
		/// <returns></returns>
		virtual bool GetParamaterValid(const std::string& name) override;

		/// <summary>
		/// Get the current value of a paramater
		/// </summary>
		/// <param name="name">the name of the paramamter</param>
		/// <returns>pair of the type and the value</returns>
		virtual std::pair<MaterialParamaterType, MaterialParamater> GetParamaterValue(const std::string& name) override;

		/// <summary>
		/// Base setting of a paramater
		/// </summary>
		/// <param name="name">the paramater name</param>
		/// <param name="value">the paramater value</param>
		virtual void SetParamater(const std::string& name, MaterialParamater value) override;

	private:
		/// <summary>
		/// Load the Material's shader from a direct string source
		/// </summary>
		/// <param name="source"></param>
		void ShaderFromString(const std::string& source);

		/// <summary>
		/// Load the Material's paramaters from a direct string source
		/// </summary>
		/// <param name="source"></param>
		void ParamatersFromString(const std::string& source);

		/// <summary>
		/// Read a text file in from source
		/// </summary>
		/// <param name="filename">the file name</param>
		/// <returns>the returned contents</returns>
		static std::string ReadSourceTextFile(const std::string& filename);

	private:
		//the actual shader
		ShaderRef m_Shader;
		//map of paramater names to the values
		std::unordered_map<std::string, std::pair<MaterialParamaterType, MaterialParamater>> m_Paramaters;
	};
}