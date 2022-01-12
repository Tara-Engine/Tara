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
		for (auto& param : m_ParamaterOverrides) {
			//dealing with textures. Maybe
			if (param.second.first == MaterialParamaterType::Sampler2D) {
				auto oldTex = std::get<Texture2DRef>(m_BaseMaterial->GetParamaterValue(param.first).second);
				auto p = oldTex->GetLastBindPoint();
				bindSlot = p.first;
			}
			Uniform u = MaterialBase::MaterialParamaterToUniform(param.second.second, param.second.first, bindSlot, 0);
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

	std::vector<std::pair<std::string, MaterialParamaterType>> MaterialInstance::GetParamaterList()
	{
		return m_BaseMaterial->GetParamaterList();
	}

	MaterialParamaterType MaterialInstance::GetParamaterType(const std::string& name)
	{
		return m_BaseMaterial->GetParamaterType(name);
	}

	bool MaterialInstance::GetParamaterValid(const std::string& name)
	{
		return m_BaseMaterial->GetParamaterValid(name);
	}

	std::pair<MaterialParamaterType, MaterialParamater> MaterialInstance::GetParamaterValue(const std::string& name)
	{
		auto iter = m_ParamaterOverrides.find(name);
		if (iter != m_ParamaterOverrides.end()) {
			return std::make_pair(iter->second.first, iter->second.second);
		}
		return m_BaseMaterial->GetParamaterValue(name);
	}

	void MaterialInstance::SetParamater(const std::string& name, MaterialParamater value)
	{
		auto t = m_BaseMaterial->GetParamaterType(name);
		if (m_BaseMaterial->GetParamaterValid(name) && MaterialBase::MaterialParamaterDataAndTypeMatch(value, t)) {
			m_ParamaterOverrides[name] = std::make_pair(t, value);
		}
		else {
			LOG_S(WARNING) << "Setting an invalid paramater '" << name << "' in a MaterialInstance blocked.";
		}
	}


}
