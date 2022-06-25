#pragma once
#include "Tara/Asset/Asset.h"
#include "Tara/Asset/Material.h"

namespace Tara {
	REFTYPE(MaterialInstance);

	class MaterialInstance : public MaterialBase, public Asset {
	public:
		MaterialInstance(MaterialBaseRef base, const std::string& name);

		~MaterialInstance() = default;

		//intentional no static Create function, as must be created from Materials directly

		/// <summary>
		/// Use this material. Binds the underlying shader and sets up the uniforms.
		/// </summary>
		virtual void Use() override;

		/// <summary>
		/// Get the material types
		/// </summary>
		/// <returns></returns>
		virtual MaterialType GetType() override;

		/// <summary>
		/// Get the shader being used.
		/// </summary>
		/// <returns></returns>
		virtual const ShaderRef& GetShader() override;

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
		const MaterialBaseRef m_BaseMaterial;
		std::unordered_map<std::string, std::pair<MaterialParameterType, MaterialParameter>> m_ParameterOverrides;
	};
}