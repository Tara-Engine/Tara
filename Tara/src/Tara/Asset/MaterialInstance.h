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
		const MaterialBaseRef m_BaseMaterial;
		std::unordered_map<std::string, std::pair<MaterialParamaterType, MaterialParamater>> m_ParamaterOverrides;
	};
}