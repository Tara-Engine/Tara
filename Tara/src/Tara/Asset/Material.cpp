#include "tarapch.h"
#include "Material.h"
#include "Tara/Asset/AssetLibrary.h"

namespace Tara{
	Material::Material(const std::string& source, Shader::SourceType sourceType, const std::string& name)
		: Asset(name)
	{
		if (sourceType == Shader::SourceType::Strings) {
			LoadFromString(source);
		}
	}

	Material::~Material()
	{
	}

	MaterialRef Material::Create(const std::string& source, Shader::SourceType sourceType, const std::string& name)
	{
		auto ref = AssetLibrary::Get()->GetAssetIf<Material>(name);
		if (ref == nullptr) {
			ref = std::make_shared<Material>(source, sourceType, name);
			AssetLibrary::Get()->RegisterAsset(ref);
		}
		return ref;
	}

	void Material::Use()
	{
		m_Shader->Bind();
		int32_t bindSlot;
		for (auto& param : m_Paramaters) {
			Uniform u = MaterialBase::MaterialParamaterToUniform(param.second.second, param.second.first, bindSlot, 0);
			m_Shader->Send(param.first, u);
		}
	}

	std::vector<std::pair<std::string, MaterialParamaterType>> Material::GetParamaterList()
	{
		std::vector<std::pair<std::string, MaterialParamaterType>> ret;
		ret.reserve(m_Paramaters.size());
		for (auto& param : m_Paramaters) {
			ret.push_back(std::make_pair(param.first, param.second.first));
		}
		return ret;
	}

	MaterialParamaterType Material::GetParamaterType(const std::string& name)
	{
		auto iter = m_Paramaters.find(name);
		if (iter != m_Paramaters.end()) {
			return iter->second.first;
		}
		LOG_S(WARNING) << "Attempted to get a Material Paramater Type of an invalid name. Name: [" << name << "]";
		return MaterialParamaterType::Float1;
	}

	bool Material::GetParamaterValid(const std::string& name)
	{
		auto iter = m_Paramaters.find(name);
		if (iter != m_Paramaters.end()) {
			return true;
		}
		return false;
	}

	std::pair<MaterialParamaterType, MaterialParamater> Material::GetParamaterValue(const std::string& name)
	{
		auto iter = m_Paramaters.find(name);
		if (iter != m_Paramaters.end()) {
			return std::make_pair(iter->second.first, iter->second.second);
		}
		LOG_S(WARNING) << "Attempted to get a Material Paramater of an invalid name. Name: [" << name << "]";
		return std::make_pair(MaterialParamaterType::Float1, MaterialParamater(0.0f));
	}

	void Material::SetParamater(const std::string& name, MaterialParamater value)
	{
		auto iter = m_Paramaters.find(name);
		if (iter == m_Paramaters.end()) {
			LOG_S(WARNING) << "Setting a Material Paramater of an unknown name. Name: [" << name << "]. This will likely cause crashes!";
			MaterialParamaterType t = MaterialBase::MaterialParamaterTypeFromData(value);
			m_Paramaters[name] = std::make_pair(t, value);
		}
		else {
			if (MaterialBase::MaterialParamaterDataAndTypeMatch(value, iter->second.first)) {
				iter->second.second = value;
			}
			else {
				LOG_S(WARNING) << "Setting a Material Paramater to the wrong datatype. Operation ignored. Name: [" << name << "]. !";
			}
		}
	}

	void Material::LoadFromString(const std::string& source)
	{
		static const std::string vertexSource = R"V0G0N(
			#version 450 core
			layout(location=0) in vec3 a_Position;
			layout(location=1) in vec3 a_Normal;
			layout(location=2) in vec4 a_Color;
			layout(location=3) in vec2 a_UV;
			
			uniform mat4 u_MatrixViewProjection;
			uniform mat4 u_MatrixModel;
			
			out vec3 v_WorldNorm;
			out vec3 v_WorldPos;
			out vec4 v_Color;
			out vec2 v_UV;
			
			void main(){
				v_WorldNorm = vec3(inverse(transpose(u_MatrixModel)) * vec4(a_Normal, 1));
				v_WorldPos = vec3(u_MatrixModel * vec4(a_Position, 1));
				v_Color = a_Color;
				
				v_UV = a_UV;
				
				gl_Position = u_MatrixViewProjection * u_MatrixModel * vec4(a_Position, 1);
			}

		)V0G0N";

		static const std::string fragmentBegin = R"V0G0N(
			#version 450 core
			layout(location=0)out vec4 color;
			
			uniform vec3 u_CameraPositionWS;
			uniform vec3 u_CameraForwardVector;
			
			in vec3 v_WorldNorm;
			in vec3 v_WorldPos;
			in vec4 v_Color;
			in vec2 v_UV;
		)V0G0N";

		//TODO when deferred rendering is implemented, this will be MASSIVELY changed
		static const std::string fragmentEnd = R"V0G0N(
			
			void main(){
				float da = dot(normalize(v_WorldNorm), normalize(u_CameraPositionWS-v_WorldPos));
				float db = dot(normalize(v_WorldNorm), normalize(u_CameraForwardVector));
				da = clamp(da, 0, 1);
				db = clamp(db, 0, 1);
				color = vec4(vec3((da + db + 1)/3), 1) * diffuse();
			
			}
		)V0G0N";

		std::string fragmentSource = fragmentBegin + source + fragmentEnd;

		std::unordered_map<Shader::TargetStage, std::string> sources;
		sources[Shader::TargetStage::Vertex] = vertexSource;
		sources[Shader::TargetStage::Pixel] = std::move(fragmentSource);
		m_Shader = Shader::Create(GetAssetName() + "_Shader", Shader::SourceType::Strings, sources);
	}














}
