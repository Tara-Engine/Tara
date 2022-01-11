#include "tarapch.h"
#include "Material.h"
#include <regex>
#include <fstream>
#include "Tara/Asset/AssetLibrary.h"

namespace Tara{

	std::unordered_map<MaterialType, std::string> Material::SourcePartsVertex = {
		{
			MaterialType::UNLIT,
			R"V0G0N(
			//UNLIT
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

			)V0G0N"
		}, 
		{
			MaterialType::LIT,
			R"V0G0N(
			//LIT
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

			)V0G0N"
		},
		{
			MaterialType::LIGHTING,
			R"V0G0N(
			//LIGHTING
			#version 450 core
			layout(location=0) in vec2 a_Position;
			layout(location=1) in vec2 a_UV;
			
			out vec2 v_UV;
			
			void main(){
				v_UV = a_UV;
				gl_Position =  vec4(a_Position, 0, 1);
			}

			)V0G0N"
		},
	};

	std::unordered_map<MaterialType, std::string> Material::SourcePartsFragmentBegin = {
		{
			MaterialType::UNLIT,
			R"V0G0N(
			//UNLIT
			#version 450 core
			layout(location=0)out vec4 outColor;
			
			uniform vec3 u_CameraPositionWS;
			uniform vec3 u_CameraForwardVector;
			
			in vec3 v_WorldNorm;
			in vec3 v_WorldPos;
			in vec4 v_Color;
			in vec2 v_UV;
			)V0G0N"
		},
		{
			MaterialType::LIT,
			R"V0G0N(
			//LIT
			#version 450 core
			layout(location=0)out vec4 ColorMetallic;
			layout(location=1)out vec4 SpecularRoughness;
			layout(location=2)out vec4 EmissiveAO;
			layout(location=3)out vec4 WorldSpaceNormal;
			layout(location=4)out vec4 WorldSpacePosition;
			
			uniform vec3 u_CameraPositionWS;
			uniform vec3 u_CameraForwardVector;
			
			in vec3 v_WorldNorm;
			in vec3 v_WorldPos;
			in vec4 v_Color;
			in vec2 v_UV;

			vec3 PixelNormal = vec3(0);
			)V0G0N"
		},
		{
			MaterialType::LIGHTING,
			R"V0G0N(
			//LIGHTING
			#version 450 core
			layout(location=0)out vec4 outColor;

			uniform sampler2D u_ColorMetallicSampler;
			uniform sampler2D u_SpecularRoughnessSampler;
			uniform sampler2D u_EmissiveAOSampler;
			uniform sampler2D u_WorldSpaceNormalSampler;
			uniform sampler2D u_WorldSpacePositionSampler;

			uniform vec2 u_TargetSize;			
			uniform vec3 u_CameraPositionWS;
			uniform vec3 u_CameraForwardVector;

			uniform int u_LightCount;
			uniform vec3 u_LightPositions[128];
			uniform vec3 u_LightForwardVectors[128]; //Pitch, Yaw, Roll
			uniform vec3 u_LightColors[128];
			uniform vec4 u_LightTypeIntensitieCustoms[128];

			in vec2 v_UV;

			float Metallic         = 0;
			float Roughness        = 0;
			float AmbientOcclusion = 0;
			vec3 Diffuse            = vec3(0);
			vec3 Specular           = vec3(0);
			vec3 Emissive           = vec3(0);
			vec3 WorldSpaceNormal   = vec3(0);
			vec3 WorldSpacePosition = vec3(0);

			const int LightType_Point		= 0;			
			const int LightType_Spot		= 1;			
			const int LightType_Directional	= 2;			
			const int LightType_Ambient		= 3;			
			const int LightType_Rect		= 4;			

			)V0G0N"
		},
	};

	std::unordered_map<MaterialType, std::string> Material::SourcePartsFragmentEnd = {
		{
			MaterialType::UNLIT,
			R"V0G0N(
			//UNLIT
			void main(){
				outColor = diffuse();
			}
			)V0G0N"
		},
		{
			MaterialType::LIT,
			R"V0G0N(
			//LIT
			void main(){
				PixelNormal = normalize(v_WorldNorm + normal().xyz);

				ColorMetallic = vec4(diffuse().xyz, metallic() + 1);
				SpecularRoughness = vec4(specular().xyz, roughness() + 1);
				EmissiveAO = vec4(emissive().xyz, ambientOcclusion() + 1);
				WorldSpaceNormal = vec4(PixelNormal,1);
				WorldSpacePosition = vec4(v_WorldPos,1);
			}
			)V0G0N"
		},
		{
			MaterialType::LIGHTING,
			R"V0G0N(
			//LIGHTING
			void main(){
				Diffuse            = texture(u_ColorMetallicSampler, v_UV).xyz;
				Metallic          = texture(u_ColorMetallicSampler, v_UV).w -1;
				Specular           = texture(u_SpecularRoughnessSampler, v_UV).xyz;
				Roughness         = texture(u_SpecularRoughnessSampler, v_UV).w -1;
				Emissive           = texture(u_EmissiveAOSampler, v_UV).xyz;
				AmbientOcclusion  = texture(u_EmissiveAOSampler, v_UV).w -1;
				WorldSpaceNormal   = texture(u_WorldSpaceNormalSampler, v_UV).xyz;
				WorldSpacePosition = texture(u_WorldSpacePositionSampler, v_UV).xyz;
				
				outColor = color();
			}
			)V0G0N"
		},
	};




	Material::Material(MaterialType type, const std::string& source, Shader::SourceType sourceType, const std::string& name)
		: Asset(name), m_Type(type)
	{
		if (sourceType == Shader::SourceType::Strings) {
			ShaderFromString(source);
			ParamatersFromString(source);
		}
		else if (sourceType == Shader::SourceType::TextFiles) {
			std::string readSource = Material::ReadSourceTextFile(source);
			ShaderFromString(readSource);
			ParamatersFromString(readSource);
		}
	}

	Material::~Material()
	{
	}

	MaterialRef Material::Create(MaterialType type, const std::string& source, Shader::SourceType sourceType, const std::string& name)
	{
		auto ref = AssetLibrary::Get()->GetAssetIf<Material>(name);
		if (ref == nullptr) {
			ref = std::make_shared<Material>(type, source, sourceType, name);
			AssetLibrary::Get()->RegisterAsset(ref);
		}
		return ref;
	}

	void Material::Use()
	{
		if (m_Type == MaterialType::LIT) {
			RenderCommand::EnableDeferred(true);
		}
		else {
			RenderCommand::EnableDeferred(false);
		}
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

	void Material::ShaderFromString(const std::string& source)
	{
		std::string typestr = "ERR";
		switch (m_Type) {
		case MaterialType::LIGHTING: {typestr = "LIGHTING"; break; }
		case MaterialType::LIT: {typestr = "LIT"; break; }
		case MaterialType::UNLIT: {typestr = "UNLIT"; break; }
		}
		LOG_S(INFO) << "Compiling Material: " << GetAssetName() << "Type: " << typestr;

		
		const std::string& vertexSource = SourcePartsVertex[m_Type];

		const std::string& fragmentBegin = SourcePartsFragmentBegin[m_Type];

		const std::string& fragmentEnd = SourcePartsFragmentEnd[m_Type];

		std::string fragmentSource = fragmentBegin + source + fragmentEnd;

		std::unordered_map<Shader::TargetStage, std::string> sources;
		sources[Shader::TargetStage::Vertex] = vertexSource;
		sources[Shader::TargetStage::Pixel] = std::move(fragmentSource);
		m_Shader = Shader::Create(GetAssetName() + "_Shader", Shader::SourceType::Strings, sources);
	}

	void Material::ParamatersFromString(const std::string& source)
	{

		std::istringstream iss(source);

		for (std::string code; std::getline(iss, code); ) {

			std::regex filter{ ".*(uniform) ([a-zA-Z][a-zA-Z0-9]+) ([a-zA-Z][a-zA-Z0-9_-]+)(\\[[0-9]+\\])?.*" };

			std::smatch sm;
			std::regex_match(code.cbegin(), code.cend(), sm, filter);

			if (sm.size() >= 4) {
				std::string typeName = sm[2];
				std::string paramName = sm[3];
				bool vector = (sm[4] != "");

				MaterialParamaterType type = MaterialBase::MaterialParamaterTypeFromString(typeName, vector);
				if (type != MaterialParamaterType::INVALID){
					m_Paramaters[paramName] = std::make_pair(type, MaterialBase::DefaultMaterialParamaterFromType(type));
				}
			}
		}

	}

	std::string Material::ReadSourceTextFile(const std::string& filename)
	{
		std::string result;
		std::ifstream in(filename, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end); //find end of file
			result.resize(in.tellg()); //resize string
			in.seekg(0, std::ios::beg); //return to begin
			in.read(&result[0], result.size()); //read file
			in.close();
		}
		else {
			ABORT_F("Error! Attempted to open an invalid material source file: %s", filename.c_str());
		}
		return result;
	}












}
