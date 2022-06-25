#include "tarapch.h"
#include "MaterialBase.h"
#include "Tara/Asset/MaterialInstance.h"
#include "Tara/Asset/AssetLibrary.h"

namespace Tara {

	MaterialParameterType MaterialBase::MaterialParameterTypeFromData(const MaterialParameter& data)
	{
		if (std::holds_alternative<float>(data)) {
			return MaterialParameterType::Float1;
		}
		else if (std::holds_alternative<glm::vec2>(data)) {
			return MaterialParameterType::Float2;
		}
		else if (std::holds_alternative<glm::vec3>(data)) {
			return MaterialParameterType::Float3;
		}
		else if (std::holds_alternative<glm::vec4>(data)) {
			return MaterialParameterType::Float4;
		}

		else if (std::holds_alternative<int32_t>(data)) {
			return MaterialParameterType::Int1;
		}
		else if (std::holds_alternative<glm::ivec2>(data)) {
			return MaterialParameterType::Int2;
		}
		else if (std::holds_alternative<glm::ivec3>(data)) {
			return MaterialParameterType::Int3;
		}
		else if (std::holds_alternative<glm::ivec4>(data)) {
			return MaterialParameterType::Int4;
		}

		else if (std::holds_alternative<uint32_t>(data)) {
			return MaterialParameterType::UInt1;
		}
		else if (std::holds_alternative<glm::uvec2>(data)) {
			return MaterialParameterType::UInt2;
		}
		else if (std::holds_alternative<glm::uvec3>(data)) {
			return MaterialParameterType::UInt3;
		}
		else if (std::holds_alternative<glm::uvec4>(data)) {
			return MaterialParameterType::UInt4;
		}

		else if (std::holds_alternative<std::vector<float>>(data)) {
			return MaterialParameterType::Float1Vector;
		}
		else if (std::holds_alternative< std::vector<glm::vec2>>(data)) {
			return MaterialParameterType::Float2Vector;
		}
		else if (std::holds_alternative< std::vector<glm::vec3>>(data)) {
			return MaterialParameterType::Float3Vector;
		}
		else if (std::holds_alternative< std::vector<glm::vec4>>(data)) {
			return MaterialParameterType::Float4Vector;
		}

		else if (std::holds_alternative< std::vector<int32_t>>(data)) {
			return MaterialParameterType::Int1Vector;
		}
		else if (std::holds_alternative< std::vector<glm::ivec2>>(data)) {
			return MaterialParameterType::Int2Vector;
		}
		else if (std::holds_alternative< std::vector<glm::ivec3>>(data)) {
			return MaterialParameterType::Int3Vector;
		}
		else if (std::holds_alternative< std::vector<glm::ivec4>>(data)) {
			return MaterialParameterType::Int4Vector;
		}

		else if (std::holds_alternative< std::vector<uint32_t>>(data)) {
			return MaterialParameterType::UInt1Vector;
		}
		else if (std::holds_alternative< std::vector<glm::uvec2>>(data)) {
			return MaterialParameterType::UInt2Vector;
		}
		else if (std::holds_alternative< std::vector<glm::uvec3>>(data)) {
			return MaterialParameterType::UInt3Vector;
		}
		else if (std::holds_alternative< std::vector<glm::uvec4>>(data)) {
			return MaterialParameterType::UInt4Vector;
		}

		else if (std::holds_alternative< std::vector<glm::mat2>>(data)) {
			return MaterialParameterType::Matrix2Vector;
		}
		else if (std::holds_alternative< std::vector<glm::mat3>>(data)) {
			return MaterialParameterType::Matrix3Vector;
		}
		else if (std::holds_alternative< std::vector<glm::mat4>>(data)) {
			return MaterialParameterType::Matrix4Vector;
		}

		else if (std::holds_alternative< std::vector<glm::mat2x3>>(data)) {
			return MaterialParameterType::Matrix2x3Vector;
		}
		else if (std::holds_alternative< std::vector<glm::mat3x2>>(data)) {
			return MaterialParameterType::Matrix3x2Vector;
		}

		else if (std::holds_alternative< std::vector<glm::mat2x4>>(data)) {
			return MaterialParameterType::Matrix2x4Vector;
		}
		else if (std::holds_alternative< std::vector<glm::mat4x2>>(data)) {
			return MaterialParameterType::Matrix4x2Vector;
		}

		else if (std::holds_alternative< std::vector<glm::mat3x4>>(data)) {
			return MaterialParameterType::Matrix3x4Vector;
		}
		else if (std::holds_alternative< std::vector<glm::mat4x3>>(data)) {
			return MaterialParameterType::Matrix4x3Vector;
		}

		else if (std::holds_alternative<Texture2DRef>(data)) {
			return MaterialParameterType::Sampler2D;
		}

		else if (std::holds_alternative<TextureCubemapRef>(data)) {
		return MaterialParameterType::SamplerCube;
		}

		LOG_S(WARNING) << "Unknown MatieralParameterType in the data";
		return MaterialParameterType::Float1;
	}

	bool MaterialBase::MaterialParameterDataAndTypeMatch(const MaterialParameter& data, MaterialParameterType type)
	{
		return MaterialParameterTypeFromData(data) == type;
	}

	Uniform MaterialBase::MaterialParameterToUniform(const MaterialParameter& data, MaterialParameterType type, int32_t& bindingA, int32_t bindingB)
	{
		UniformData uniformData{ 1.0f };
		UniformType uniformType = UniformType::Float1;
		//Select the correct data and type format for the Uniform, performing any conversions or bindings nesecary.
		switch (type) {
		case MaterialParameterType::Float1: {
			uniformType = UniformType::Float1;
			uniformData.Float1 = std::get<float>(data);
			break;
		}
		case MaterialParameterType::Float2: {
			uniformType = UniformType::Float2;
			uniformData.Float2 = std::get<glm::vec2>(data);
			break;
		}
		case MaterialParameterType::Float3: {
			uniformType = UniformType::Float3;
			uniformData.Float3 = std::get<glm::vec3>(data);
			break;
		}
		case MaterialParameterType::Float4: {
			uniformType = UniformType::Float4;
			uniformData.Float4 = std::get<glm::vec4>(data);
			break;
		}

		case MaterialParameterType::Int1: {
			uniformType = UniformType::Int1;
			uniformData.Int1 = std::get<int32_t>(data);
			break;
		}
		case MaterialParameterType::Int2: {
			uniformType = UniformType::Int2;
			uniformData.Int2 = std::get<glm::ivec2>(data);
			break;
		}
		case MaterialParameterType::Int3: {
			uniformType = UniformType::Int3;
			uniformData.Int3 = std::get<glm::ivec3>(data);
			break;
		}
		case MaterialParameterType::Int4: {
			uniformType = UniformType::Int4;
			uniformData.Int4 = std::get<glm::ivec4>(data);
			break;
		}

		case MaterialParameterType::UInt1: {
			uniformType = UniformType::UInt1;
			uniformData.UInt1 = std::get<uint32_t>(data);
			break;
		}
		case MaterialParameterType::UInt2: {
			uniformType = UniformType::UInt2;
			uniformData.UInt2 = std::get<glm::uvec2>(data);
			break;
		}
		case MaterialParameterType::UInt3: {
			uniformType = UniformType::UInt3;
			uniformData.UInt3 = std::get<glm::uvec3>(data);
			break;
		}
		case MaterialParameterType::UInt4: {
			uniformType = UniformType::UInt4;
			uniformData.UInt4 = std::get<glm::uvec4>(data);
			break;
		}

		case MaterialParameterType::Float1Vector: {
			uniformType = UniformType::Float1Vector;
			auto& vec = std::get<std::vector<float>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ vec.data(), 1, (int32_t)vec.size() };
			break;
		}
		case MaterialParameterType::Float2Vector: {
			uniformType = UniformType::Float2Vector;
			auto& vec = std::get<std::vector<glm::vec2>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 2, (int32_t)vec.size() };
			break;
		}
		case MaterialParameterType::Float3Vector: {
			uniformType = UniformType::Float3Vector;
			auto& vec = std::get<std::vector<glm::vec3>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 3, (int32_t)vec.size() };
			break;
		}
		case MaterialParameterType::Float4Vector: {
			uniformType = UniformType::Float4Vector;
			auto& vec = std::get<std::vector<glm::vec4>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 4, (int32_t)vec.size() };
			break;
		}

		case MaterialParameterType::Int1Vector: {
			uniformType = UniformType::Int1Vector;
			auto& vec = std::get<std::vector<int32_t>>(data);
			uniformData.IntVector = UniformData::IntVectorType{ vec.data(), 1, (int32_t)vec.size() };
			break;
		}
		case MaterialParameterType::Int2Vector: {
			uniformType = UniformType::Int2Vector;
			auto& vec = std::get<std::vector<glm::ivec2>>(data);
			uniformData.IntVector = UniformData::IntVectorType{ (int32_t*)vec.data(), 2, (int32_t)vec.size() };
			break;
		}
		case MaterialParameterType::Int3Vector: {
			uniformType = UniformType::Int3Vector;
			auto& vec = std::get<std::vector<glm::ivec3>>(data);
			uniformData.IntVector = UniformData::IntVectorType{ (int32_t*)vec.data(), 3, (int32_t)vec.size() };
			break;
		}
		case MaterialParameterType::Int4Vector: {
			uniformType = UniformType::Int4Vector;
			auto& vec = std::get<std::vector<glm::ivec4>>(data);
			uniformData.IntVector = UniformData::IntVectorType{ (int32_t*)vec.data(), 4, (int32_t)vec.size() };
			break;
		}

		case MaterialParameterType::UInt1Vector: {
			uniformType = UniformType::UInt1Vector;
			auto& vec = std::get<std::vector<uint32_t>>(data);
			uniformData.UIntVector = UniformData::UIntVectorType{ vec.data(), 1, (int32_t)vec.size() };
			break;
		}
		case MaterialParameterType::UInt2Vector: {
			uniformType = UniformType::UInt2Vector;
			auto& vec = std::get<std::vector<glm::uvec2>>(data);
			uniformData.UIntVector = UniformData::UIntVectorType{ (uint32_t*)vec.data(), 2, (int32_t)vec.size() };
			break;
		}
		case MaterialParameterType::UInt3Vector: {
			uniformType = UniformType::UInt3Vector;
			auto& vec = std::get<std::vector<glm::ivec3>>(data);
			uniformData.UIntVector = UniformData::UIntVectorType{ (uint32_t*)vec.data(), 3, (int32_t)vec.size() };
			break;
		}
		case MaterialParameterType::UInt4Vector: {
			uniformType = UniformType::UInt4Vector;
			auto& vec = std::get<std::vector<glm::ivec4>>(data);
			uniformData.UIntVector = UniformData::UIntVectorType{ (uint32_t*)vec.data(), 4, (int32_t)vec.size() };
			break;
		}


		case MaterialParameterType::Matrix2Vector: {
			uniformType = UniformType::Matrix2Vector;
			auto& vec = std::get<std::vector<glm::mat2>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 4, (int32_t)vec.size() };
			break;
		}
		case MaterialParameterType::Matrix3Vector: {
			uniformType = UniformType::Matrix3Vector;
			auto& vec = std::get<std::vector<glm::mat3>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 9, (int32_t)vec.size() };
			break;
		}
		case MaterialParameterType::Matrix4Vector: {
			uniformType = UniformType::Matrix4Vector;
			auto& vec = std::get<std::vector<glm::mat4>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 16, (int32_t)vec.size() };
			break;
		}

		case MaterialParameterType::Matrix2x3Vector: {
			uniformType = UniformType::Matrix2x3Vector;
			auto& vec = std::get<std::vector<glm::mat2x3>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 6, (int32_t)vec.size() };
			break;
		}
		case MaterialParameterType::Matrix3x2Vector: {
			uniformType = UniformType::Matrix3x2Vector;
			auto& vec = std::get<std::vector<glm::mat3x2>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 6, (int32_t)vec.size() };
			break;
		}

		case MaterialParameterType::Matrix2x4Vector: {
			uniformType = UniformType::Matrix2x4Vector;
			auto& vec = std::get<std::vector<glm::mat2x4>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 8, (int32_t)vec.size() };
			break;
		}
		case MaterialParameterType::Matrix4x2Vector: {
			uniformType = UniformType::Matrix4x2Vector;
			auto& vec = std::get<std::vector<glm::mat4x2>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 8, (int32_t)vec.size() };
			break;
		}

		case MaterialParameterType::Matrix3x4Vector: {
			uniformType = UniformType::Matrix3x4Vector;
			auto& vec = std::get<std::vector<glm::mat3x4>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 12, (int32_t)vec.size() };
			break;
		}
		case MaterialParameterType::Matrix4x3Vector: {
			uniformType = UniformType::Matrix4x3Vector;
			auto& vec = std::get<std::vector<glm::mat4x3>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 12, (int32_t)vec.size() };
			break;
		}

		case MaterialParameterType::Sampler2D: {
			uniformType = UniformType::Int1;
			uniformData.Int1 = bindingA;
			auto& ref = std::get<Texture2DRef>(data);
			if (ref) {
				ref->Bind(bindingA++, bindingB);
			}
			break;
		}
		case MaterialParameterType::SamplerCube: {
			uniformType = UniformType::Int1;
			uniformData.Int1 = bindingA;
			auto& ref = std::get<TextureCubemapRef>(data);
			if (ref) {
				ref->Bind(bindingA++, bindingB);
			}
			break;
		}

		default: {
			uniformType = UniformType::Float1;
			uniformData.Float1 = 0;
		}

		}

		//OK, now, with the uniformType and uniformData finally selected, we can move on and make the Uniform
		return Uniform{ uniformType, std::move(uniformData) };
	}


	MaterialParameterType MaterialBase::MaterialParameterTypeFromString(const std::string type, bool isVector)
	{
		if (type == "float") {
			if (isVector) { return MaterialParameterType::Float1Vector; }
			else { return MaterialParameterType::Float1; }
		}
		else if (type == "vec2") {
			if (isVector) { return MaterialParameterType::Float2Vector; }
			else { return MaterialParameterType::Float2; }
		}
		else if (type == "vec3") {
			if (isVector) { return MaterialParameterType::Float3Vector; }
			else { return MaterialParameterType::Float3; }
		}
		else if (type == "vec4") {
			if (isVector) { return MaterialParameterType::Float4Vector; }
			else { return MaterialParameterType::Float4; }
		}
		else if (type == "int") {
			if (isVector) { return MaterialParameterType::Int1Vector; }
			else { return MaterialParameterType::Int1; }
		}
		else if (type == "ivec2") {
			if (isVector) { return MaterialParameterType::Int2Vector; }
			else { return MaterialParameterType::Int2; }
		}
		else if (type == "ivec3") {
			if (isVector) { return MaterialParameterType::Int3Vector; }
			else { return MaterialParameterType::Int3; }
		}
		else if (type == "ivec4") {
			if (isVector) { return MaterialParameterType::Int4Vector; }
			else { return MaterialParameterType::Int4; }
		}
		else if (type == "uint") {
			if (isVector) { return MaterialParameterType::UInt1Vector; }
			else { return MaterialParameterType::UInt1; }
		}
		else if (type == "uvec2") {
			if (isVector) { return MaterialParameterType::UInt2Vector; }
			else { return MaterialParameterType::UInt2; }
		}
		else if (type == "uvec3") {
			if (isVector) { return MaterialParameterType::UInt3Vector; }
			else { return MaterialParameterType::UInt3; }
		}
		else if (type == "uvec4") {
			if (isVector) { return MaterialParameterType::UInt4Vector; }
			else { return MaterialParameterType::UInt4; }
		}
		else if (type == "mat2") {
			return MaterialParameterType::Matrix2Vector;
		}
		else if (type == "mat3") {
			return MaterialParameterType::Matrix3Vector;
		}
		else if (type == "mat4") {
			return MaterialParameterType::Matrix4Vector;
		}
		else if (type == "mat2x3") {
			return MaterialParameterType::Matrix2x3Vector;
		}
		else if (type == "mat3x2") {
			return MaterialParameterType::Matrix3x2Vector;
		}
		else if (type == "mat2x4") {
			return MaterialParameterType::Matrix2x4Vector;
		}
		else if (type == "mat4x2") {
			return MaterialParameterType::Matrix4x2Vector;
		}
		else if (type == "mat3x4") {
			return MaterialParameterType::Matrix3x4Vector;
		}
		else if (type == "mat4x3") {
			return MaterialParameterType::Matrix4x3Vector;
		}
		else if (type == "sampler1D" || type == "isampler1D" || type == "usampler1D") {
			return MaterialParameterType::Sampler1D;
		}
		else if (type == "sampler2D" || type == "isampler2D" || type == "usampler2D") {
			return MaterialParameterType::Sampler2D;
		}
		else if (type == "sampler3D" || type == "isampler3D" || type == "usampler3D") {
			return MaterialParameterType::Sampler3D;
		}
		else if (type == "samplerCube" || type == "isamplerCube" || type == "usamplerCube") {
			return MaterialParameterType::SamplerCube;
		}
		else if (type == "sampler1DArray" || type == "isampler1DArray" || type == "usampler1DArray") {
			return MaterialParameterType::Sampler1DArray;
		}
		else if (type == "sampler2DArray" || type == "isampler2DArray" || type == "usampler2DArray") {
			return MaterialParameterType::Sampler2DArray;
		}
		else {
			LOG_S(WARNING) << "Unknown Material Parameter Type : '" << type << "'. If you are using a custom struct, don't do this!";
			return MaterialParameterType::INVALID;
		}
	}

	MaterialParameter MaterialBase::DefaultMaterialParameterFromType(MaterialParameterType type)
	{
		switch (type) {
		case MaterialParameterType::Float1: return 0.0f;
		case MaterialParameterType::Float2: return glm::vec2(0);
		case MaterialParameterType::Float3: return glm::vec3(0);
		case MaterialParameterType::Float4: return glm::vec4(0);
		case MaterialParameterType::Int1: return 0;
		case MaterialParameterType::Int2: return glm::ivec2(0);
		case MaterialParameterType::Int3: return glm::ivec3(0);
		case MaterialParameterType::Int4: return glm::ivec4(0);
		case MaterialParameterType::UInt1: return 0u;
		case MaterialParameterType::UInt2: return glm::uvec2(0);
		case MaterialParameterType::UInt3: return glm::uvec3(0);
		case MaterialParameterType::UInt4: return glm::uvec4(0);
			
		case MaterialParameterType::Float1Vector: return std::vector<float>();
		case MaterialParameterType::Float2Vector: return std::vector<glm::vec2>();
		case MaterialParameterType::Float3Vector: return std::vector<glm::vec3>();
		case MaterialParameterType::Float4Vector: return std::vector<glm::vec4>();
		case MaterialParameterType::Int1Vector: return std::vector<int32_t>();
		case MaterialParameterType::Int2Vector: return std::vector<glm::ivec2>();
		case MaterialParameterType::Int3Vector: return std::vector<glm::ivec3>();
		case MaterialParameterType::Int4Vector: return std::vector<glm::ivec4>();
		case MaterialParameterType::UInt1Vector: return std::vector<uint32_t>();
		case MaterialParameterType::UInt2Vector: return std::vector<glm::uvec2>();
		case MaterialParameterType::UInt3Vector: return std::vector<glm::uvec3>();
		case MaterialParameterType::UInt4Vector: return std::vector<glm::uvec4>();

		case MaterialParameterType::Matrix2Vector: return std::vector<glm::mat2>();
		case MaterialParameterType::Matrix3Vector: return std::vector<glm::mat3>();
		case MaterialParameterType::Matrix4Vector: return std::vector<glm::mat4>();
		
		case MaterialParameterType::Matrix2x3Vector: return std::vector<glm::mat2x3>();
		case MaterialParameterType::Matrix3x2Vector: return std::vector<glm::mat3x2>();
		case MaterialParameterType::Matrix2x4Vector: return std::vector<glm::mat2x4>();
		case MaterialParameterType::Matrix4x2Vector: return std::vector<glm::mat4x2>();
		case MaterialParameterType::Matrix3x4Vector: return std::vector<glm::mat3x4>();
		case MaterialParameterType::Matrix4x3Vector: return std::vector<glm::mat4x3>();

		case MaterialParameterType::Sampler2D: return Texture2DRef(nullptr);
		case MaterialParameterType::SamplerCube: return TextureCubemapRef(nullptr);

		default: return 0.0f;
		}
	}

	MaterialInstanceRef MaterialBase::CreateInstance(const std::string name)
	{
		auto ref = AssetLibrary::Get()->GetAssetIf<MaterialInstance>(name);
		if (ref == nullptr) {
			ref = std::make_shared<MaterialInstance>(shared_from_this(), name);
			AssetLibrary::Get()->RegisterAsset(ref);
			return ref;
		}
		LOG_S(WARNING) << "Attemtped to create a MaterialInstance with an already existing name";
		return nullptr;
	}

	MaterialInstanceRef MaterialBase::CreateInstanceUnregistered()
	{
		return std::make_shared<MaterialInstance>(shared_from_this(), "__UNREGISTERED__");
	}

}