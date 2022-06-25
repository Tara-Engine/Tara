#include "tarapch.h"
#include "Material.h"
#include <regex>
#include <fstream>
#include "Tara/Asset/AssetLibrary.h"

namespace Tara{

	Material::Material(MaterialType type, const std::string& source, Shader::SourceType sourceType, const std::string& name)
		: Asset(name), m_Type(type)
	{
		if (sourceType == Shader::SourceType::Strings) {
			ShaderFromString(source);
			ParametersFromString(source);
		}
		else if (sourceType == Shader::SourceType::TextFiles) {
			std::string readSource = Material::ReadSourceTextFile(source);
			ShaderFromString(readSource);
			ParametersFromString(readSource);
		}
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
		int32_t bindSlot = 0;
		for (auto& param : m_Parameters) {
			Uniform u = MaterialBase::MaterialParameterToUniform(param.second.second, param.second.first, bindSlot, 0);
			m_Shader->Send(param.first, u);
		}
	}

	std::vector<std::pair<std::string, MaterialParameterType>> Material::GetParameterList()
	{
		std::vector<std::pair<std::string, MaterialParameterType>> ret;
		ret.reserve(m_Parameters.size());
		for (auto& param : m_Parameters) {
			ret.push_back(std::make_pair(param.first, param.second.first));
		}
		return ret;
	}

	MaterialParameterType Material::GetParameterType(const std::string& name)
	{
		auto iter = m_Parameters.find(name);
		if (iter != m_Parameters.end()) {
			return iter->second.first;
		}
		LOG_S(WARNING) << "Attempted to get a Material Parameter Type of an invalid name. Name: [" << name << "]";
		return MaterialParameterType::Float1;
	}

	bool Material::GetParameterValid(const std::string& name)
	{
		auto iter = m_Parameters.find(name);
		if (iter != m_Parameters.end()) {
			return true;
		}
		return false;
	}

	std::pair<MaterialParameterType, MaterialParameter> Material::GetParameterValue(const std::string& name)
	{
		auto iter = m_Parameters.find(name);
		if (iter != m_Parameters.end()) {
			return std::make_pair(iter->second.first, iter->second.second);
		}
		LOG_S(WARNING) << "Attempted to get a Material Parameter of an invalid name. Name: [" << name << "]";
		return std::make_pair(MaterialParameterType::Float1, MaterialParameter(0.0f));
	}

	void Material::SetParameter(const std::string& name, MaterialParameter value)
	{
		auto iter = m_Parameters.find(name);
		if (iter == m_Parameters.end()) {
			LOG_S(WARNING) << "Setting a Material Parameter of an unknown name. Name: [" << name << "]. This will likely cause crashes!";
			MaterialParameterType t = MaterialBase::MaterialParameterTypeFromData(value);
			m_Parameters[name] = std::make_pair(t, value);
		}
		else {
			if (MaterialBase::MaterialParameterDataAndTypeMatch(value, iter->second.first)) {
				iter->second.second = value;
			}
			else {
				LOG_S(WARNING) << "Setting a Material Parameter to the wrong datatype. Operation ignored. Name: [" << name << "]. !";
			}
		}
	}

	std::string Material::PreprocessSource(const std::string& source)
	{
		std::istringstream iss(source);
		std::stringstream oss;

		for (std::string code; std::getline(iss, code); ) {
			std::regex filter{ "#include <([a-z]*)>.*" };

			std::smatch sm;
			std::regex_match(code.cbegin(), code.cend(), sm, filter);

			if (sm.size() >= 2) {
				std::string incCode = sm[1];
				LOG_S(INFO) << "Material:: Replacing the line '" << sm[0] << "' with the corresponding code [" << incCode << "]";
				//include the code for that name
				auto it = SourceIncludes.find(incCode);
				if (it != SourceIncludes.end()) {
					oss << it->second;
				}
				else {
					LOG_S(WARNING) << "Material:: Unable to find an include of '" << incCode << "'. Removing this line and continuing.";
				}
			}
			else {
				oss << code << std::endl;
			}
		}

		return oss.str();
	}

	void Material::ShaderFromString(const std::string& source)
	{
		std::string typestr = "ERR";
		switch (m_Type) {
		case MaterialType::LIGHTING: {typestr = "LIGHTING"; break; }
		case MaterialType::LIT: {typestr = "LIT"; break; }
		case MaterialType::UNLIT: {typestr = "UNLIT"; break; }
		case MaterialType::POSTPROCESS: {typestr = "POSTPROCESS"; break; }
		}
		LOG_S(INFO) << "Compiling Material: " << GetAssetName() << "Type: " << typestr;

		
		const std::string& vertexSource = SourcePartsVertex[m_Type];

		const std::string& fragmentBegin = SourcePartsFragmentBegin[m_Type];

		const std::string& fragmentEnd = SourcePartsFragmentEnd[m_Type];

		std::string fragmentSource = fragmentBegin + PreprocessSource(source) + fragmentEnd;
		
		std::unordered_map<Shader::TargetStage, std::string> sources;
		sources[Shader::TargetStage::Vertex] = vertexSource;
		sources[Shader::TargetStage::Pixel] = std::move(fragmentSource);
		m_Shader = Shader::Create(GetAssetName() + "_Shader", Shader::SourceType::Strings, sources);
	}

	void Material::ParametersFromString(const std::string& source)
	{

		std::istringstream iss(source);

		for (std::string code; std::getline(iss, code); ) {

			std::regex filter{ ".*(uniform) ([a-zA-Z][a-zA-Z0-9]+) ([a-zA-Z][a-zA-Z0-9_-]+)(\\[[0-9]+\\])?.*" };

			std::smatch sm;
			std::regex_match(code.cbegin(), code.cend(), sm, filter);
			//the check for '/' is to eliminate comments. YES, this can be bypassed with a space starting the line.
			if (sm.size() >= 4 && code[0] != '/') {
				std::string typeName = sm[2];
				std::string paramName = sm[3];
				bool vector = (sm[4] != "");

				MaterialParameterType type = MaterialBase::MaterialParameterTypeFromString(typeName, vector);
				if (type != MaterialParameterType::INVALID){
					m_Parameters[paramName] = std::make_pair(type, MaterialBase::DefaultMaterialParameterFromType(type));
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
