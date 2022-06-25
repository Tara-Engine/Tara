#include "tarapch.h"
#include "MaterialInstance.h"
#include "Tara/Renderer/Texture.h"

namespace Tara{
	MaterialInstance::MaterialInstance(MaterialBaseRef base, const std::string& name)
		: Asset(name), m_BaseMaterial(base)
	{}

	void MaterialInstance::Use()
	{
		m_BaseMaterial->Use();
		int32_t bindSlot = 0;
		for (auto& param : m_ParameterOverrides) {
			//dealing with textures. Maybe
			if (param.second.first == MaterialParameterType::Sampler2D) {
				auto oldTex = std::get<Texture2DRef>(m_BaseMaterial->GetParameterValue(param.first).second);
				auto p = oldTex->GetLastBindPoint();
				bindSlot = p.first;
			}
			Uniform u = MaterialBase::MaterialParameterToUniform(param.second.second, param.second.first, bindSlot, 0);
			m_BaseMaterial->GetShader()->Send(param.first, u);
		}
	}

	MaterialType MaterialInstance::GetType()
	{
		return m_BaseMaterial->GetType();
	}

	const ShaderRef& MaterialInstance::GetShader()
	{
		return m_BaseMaterial->GetShader();
	}

	std::vector<std::pair<std::string, MaterialParameterType>> MaterialInstance::GetParameterList()
	{
		return m_BaseMaterial->GetParameterList();
	}

	MaterialParameterType MaterialInstance::GetParameterType(const std::string& name)
	{
		return m_BaseMaterial->GetParameterType(name);
	}

	bool MaterialInstance::GetParameterValid(const std::string& name)
	{
		return m_BaseMaterial->GetParameterValid(name);
	}

	std::pair<MaterialParameterType, MaterialParameter> MaterialInstance::GetParameterValue(const std::string& name)
	{
		auto iter = m_ParameterOverrides.find(name);
		if (iter != m_ParameterOverrides.end()) {
			return std::make_pair(iter->second.first, iter->second.second);
		}
		return m_BaseMaterial->GetParameterValue(name);
	}

	void MaterialInstance::SetParameter(const std::string& name, MaterialParameter value)
	{
		auto t = m_BaseMaterial->GetParameterType(name);
		if (m_BaseMaterial->GetParameterValid(name) && MaterialBase::MaterialParameterDataAndTypeMatch(value, t)) {
			m_ParameterOverrides[name] = std::make_pair(t, value);
		}
		else {
			LOG_S(WARNING) << "Setting an invalid Parameter '" << name << "' in a MaterialInstance blocked.";
		}
	}


}
