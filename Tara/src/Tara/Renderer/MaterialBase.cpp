#include "tarapch.h"
#include "MaterialBase.h"

namespace Tara {

	MaterialParamaterType MaterialBase::MaterialParamaterTypeFromData(const MaterialParamater& data)
	{
		if (std::holds_alternative<float>(data)) {
			return MaterialParamaterType::Float1;
		}
		else if (std::holds_alternative<glm::vec2>(data)) {
			return MaterialParamaterType::Float2;
		}
		else if (std::holds_alternative<glm::vec3>(data)) {
			return MaterialParamaterType::Float3;
		}
		else if (std::holds_alternative<glm::vec4>(data)) {
			return MaterialParamaterType::Float4;
		}

		else if (std::holds_alternative<int32_t>(data)) {
			return MaterialParamaterType::Int1;
		}
		else if (std::holds_alternative<glm::ivec2>(data)) {
			return MaterialParamaterType::Int2;
		}
		else if (std::holds_alternative<glm::ivec3>(data)) {
			return MaterialParamaterType::Int3;
		}
		else if (std::holds_alternative<glm::ivec4>(data)) {
			return MaterialParamaterType::Int4;
		}

		else if (std::holds_alternative<uint32_t>(data)) {
			return MaterialParamaterType::UInt1;
		}
		else if (std::holds_alternative<glm::uvec2>(data)) {
			return MaterialParamaterType::UInt2;
		}
		else if (std::holds_alternative<glm::uvec3>(data)) {
			return MaterialParamaterType::UInt3;
		}
		else if (std::holds_alternative<glm::uvec4>(data)) {
			return MaterialParamaterType::UInt4;
		}

		else if (std::holds_alternative<std::vector<float>>(data)) {
			return MaterialParamaterType::Float1Vector;
		}
		else if (std::holds_alternative< std::vector<glm::vec2>>(data)) {
			return MaterialParamaterType::Float2Vector;
		}
		else if (std::holds_alternative< std::vector<glm::vec3>>(data)) {
			return MaterialParamaterType::Float3Vector;
		}
		else if (std::holds_alternative< std::vector<glm::vec4>>(data)) {
			return MaterialParamaterType::Float4Vector;
		}

		else if (std::holds_alternative< std::vector<int32_t>>(data)) {
			return MaterialParamaterType::Int1Vector;
		}
		else if (std::holds_alternative< std::vector<glm::ivec2>>(data)) {
			return MaterialParamaterType::Int2Vector;
		}
		else if (std::holds_alternative< std::vector<glm::ivec3>>(data)) {
			return MaterialParamaterType::Int3Vector;
		}
		else if (std::holds_alternative< std::vector<glm::ivec4>>(data)) {
			return MaterialParamaterType::Int4Vector;
		}

		else if (std::holds_alternative< std::vector<uint32_t>>(data)) {
			return MaterialParamaterType::UInt1Vector;
		}
		else if (std::holds_alternative< std::vector<glm::uvec2>>(data)) {
			return MaterialParamaterType::UInt2Vector;
		}
		else if (std::holds_alternative< std::vector<glm::uvec3>>(data)) {
			return MaterialParamaterType::UInt3Vector;
		}
		else if (std::holds_alternative< std::vector<glm::uvec4>>(data)) {
			return MaterialParamaterType::UInt4Vector;
		}

		else if (std::holds_alternative< std::vector<glm::mat2>>(data)) {
			return MaterialParamaterType::Matrix2Vector;
		}
		else if (std::holds_alternative< std::vector<glm::mat3>>(data)) {
			return MaterialParamaterType::Matrix3Vector;
		}
		else if (std::holds_alternative< std::vector<glm::mat4>>(data)) {
			return MaterialParamaterType::Matrix4Vector;
		}

		else if (std::holds_alternative< std::vector<glm::mat2x3>>(data)) {
			return MaterialParamaterType::Matrix2x3Vector;
		}
		else if (std::holds_alternative< std::vector<glm::mat3x2>>(data)) {
			return MaterialParamaterType::Matrix3x2Vector;
		}

		else if (std::holds_alternative< std::vector<glm::mat2x4>>(data)) {
			return MaterialParamaterType::Matrix2x4Vector;
		}
		else if (std::holds_alternative< std::vector<glm::mat4x2>>(data)) {
			return MaterialParamaterType::Matrix4x2Vector;
		}

		else if (std::holds_alternative< std::vector<glm::mat3x4>>(data)) {
			return MaterialParamaterType::Matrix3x4Vector;
		}
		else if (std::holds_alternative< std::vector<glm::mat4x3>>(data)) {
			return MaterialParamaterType::Matrix4x3Vector;
		}

		else if (std::holds_alternative<Texture2DRef>(data)) {
			return MaterialParamaterType::Sampler2D;
		}

		LOG_S(WARNING) << "Unknown MatieralParamaterType in the data";
		return MaterialParamaterType::Float1;
	}

	bool MaterialBase::MaterialParamaterDataAndTypeMatch(const MaterialParamater& data, MaterialParamaterType type)
	{
		return MaterialParamaterTypeFromData(data) == type;
	}

	Uniform MaterialBase::MaterialParamaterToUniform(const MaterialParamater& data, MaterialParamaterType type, int32_t& bindingA, int32_t bindingB)
	{
		UniformData uniformData{ 1.0f };
		UniformType uniformType = UniformType::Float1;
		//Select the correct data and type format for the Uniform, performing any conversions or bindings nesecary.
		switch (type) {
		case MaterialParamaterType::Float1: {
			uniformType = UniformType::Float1;
			uniformData.Float1 = std::get<float>(data);
			break;
		}
		case MaterialParamaterType::Float2: {
			uniformType = UniformType::Float2;
			uniformData.Float2 = std::get<glm::vec2>(data);
			break;
		}
		case MaterialParamaterType::Float3: {
			uniformType = UniformType::Float3;
			uniformData.Float3 = std::get<glm::vec3>(data);
			break;
		}
		case MaterialParamaterType::Float4: {
			uniformType = UniformType::Float4;
			uniformData.Float4 = std::get<glm::vec4>(data);
			break;
		}

		case MaterialParamaterType::Int1: {
			uniformType = UniformType::Int1;
			uniformData.Int1 = std::get<int32_t>(data);
			break;
		}
		case MaterialParamaterType::Int2: {
			uniformType = UniformType::Int2;
			uniformData.Int2 = std::get<glm::ivec2>(data);
			break;
		}
		case MaterialParamaterType::Int3: {
			uniformType = UniformType::Int3;
			uniformData.Int3 = std::get<glm::ivec3>(data);
			break;
		}
		case MaterialParamaterType::Int4: {
			uniformType = UniformType::Int4;
			uniformData.Int4 = std::get<glm::ivec4>(data);
			break;
		}

		case MaterialParamaterType::UInt1: {
			uniformType = UniformType::UInt1;
			uniformData.UInt1 = std::get<uint32_t>(data);
			break;
		}
		case MaterialParamaterType::UInt2: {
			uniformType = UniformType::UInt2;
			uniformData.UInt2 = std::get<glm::uvec2>(data);
			break;
		}
		case MaterialParamaterType::UInt3: {
			uniformType = UniformType::UInt3;
			uniformData.UInt3 = std::get<glm::uvec3>(data);
			break;
		}
		case MaterialParamaterType::UInt4: {
			uniformType = UniformType::UInt4;
			uniformData.UInt4 = std::get<glm::uvec4>(data);
			break;
		}

		case MaterialParamaterType::Float1Vector: {
			uniformType = UniformType::Float1Vector;
			auto& vec = std::get<std::vector<float>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ vec.data(), 1, (int32_t)vec.size() };
			break;
		}
		case MaterialParamaterType::Float2Vector: {
			uniformType = UniformType::Float2Vector;
			auto& vec = std::get<std::vector<glm::vec2>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 2, (int32_t)vec.size() };
			break;
		}
		case MaterialParamaterType::Float3Vector: {
			uniformType = UniformType::Float3Vector;
			auto& vec = std::get<std::vector<glm::vec3>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 3, (int32_t)vec.size() };
			break;
		}
		case MaterialParamaterType::Float4Vector: {
			uniformType = UniformType::Float4Vector;
			auto& vec = std::get<std::vector<glm::vec4>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 4, (int32_t)vec.size() };
			break;
		}

		case MaterialParamaterType::Int1Vector: {
			uniformType = UniformType::Int1Vector;
			auto& vec = std::get<std::vector<int32_t>>(data);
			uniformData.IntVector = UniformData::IntVectorType{ vec.data(), 1, (int32_t)vec.size() };
			break;
		}
		case MaterialParamaterType::Int2Vector: {
			uniformType = UniformType::Int2Vector;
			auto& vec = std::get<std::vector<glm::ivec2>>(data);
			uniformData.IntVector = UniformData::IntVectorType{ (int32_t*)vec.data(), 2, (int32_t)vec.size() };
			break;
		}
		case MaterialParamaterType::Int3Vector: {
			uniformType = UniformType::Int3Vector;
			auto& vec = std::get<std::vector<glm::ivec3>>(data);
			uniformData.IntVector = UniformData::IntVectorType{ (int32_t*)vec.data(), 3, (int32_t)vec.size() };
			break;
		}
		case MaterialParamaterType::Int4Vector: {
			uniformType = UniformType::Int4Vector;
			auto& vec = std::get<std::vector<glm::ivec4>>(data);
			uniformData.IntVector = UniformData::IntVectorType{ (int32_t*)vec.data(), 4, (int32_t)vec.size() };
			break;
		}

		case MaterialParamaterType::UInt1Vector: {
			uniformType = UniformType::UInt1Vector;
			auto& vec = std::get<std::vector<uint32_t>>(data);
			uniformData.UIntVector = UniformData::UIntVectorType{ vec.data(), 1, (int32_t)vec.size() };
			break;
		}
		case MaterialParamaterType::UInt2Vector: {
			uniformType = UniformType::UInt2Vector;
			auto& vec = std::get<std::vector<glm::uvec2>>(data);
			uniformData.UIntVector = UniformData::UIntVectorType{ (uint32_t*)vec.data(), 2, (int32_t)vec.size() };
			break;
		}
		case MaterialParamaterType::UInt3Vector: {
			uniformType = UniformType::UInt3Vector;
			auto& vec = std::get<std::vector<glm::ivec3>>(data);
			uniformData.UIntVector = UniformData::UIntVectorType{ (uint32_t*)vec.data(), 3, (int32_t)vec.size() };
			break;
		}
		case MaterialParamaterType::UInt4Vector: {
			uniformType = UniformType::UInt4Vector;
			auto& vec = std::get<std::vector<glm::ivec4>>(data);
			uniformData.UIntVector = UniformData::UIntVectorType{ (uint32_t*)vec.data(), 4, (int32_t)vec.size() };
			break;
		}


		case MaterialParamaterType::Matrix2Vector: {
			uniformType = UniformType::Matrix2Vector;
			auto& vec = std::get<std::vector<glm::mat2>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 4, (int32_t)vec.size() };
			break;
		}
		case MaterialParamaterType::Matrix3Vector: {
			uniformType = UniformType::Matrix3Vector;
			auto& vec = std::get<std::vector<glm::mat3>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 9, (int32_t)vec.size() };
			break;
		}
		case MaterialParamaterType::Matrix4Vector: {
			uniformType = UniformType::Matrix4Vector;
			auto& vec = std::get<std::vector<glm::mat4>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 16, (int32_t)vec.size() };
			break;
		}

		case MaterialParamaterType::Matrix2x3Vector: {
			uniformType = UniformType::Matrix2x3Vector;
			auto& vec = std::get<std::vector<glm::mat2x3>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 6, (int32_t)vec.size() };
			break;
		}
		case MaterialParamaterType::Matrix3x2Vector: {
			uniformType = UniformType::Matrix3x2Vector;
			auto& vec = std::get<std::vector<glm::mat3x2>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 6, (int32_t)vec.size() };
			break;
		}

		case MaterialParamaterType::Matrix2x4Vector: {
			uniformType = UniformType::Matrix2x4Vector;
			auto& vec = std::get<std::vector<glm::mat2x4>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 8, (int32_t)vec.size() };
			break;
		}
		case MaterialParamaterType::Matrix4x2Vector: {
			uniformType = UniformType::Matrix4x2Vector;
			auto& vec = std::get<std::vector<glm::mat4x2>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 8, (int32_t)vec.size() };
			break;
		}

		case MaterialParamaterType::Matrix3x4Vector: {
			uniformType = UniformType::Matrix3x4Vector;
			auto& vec = std::get<std::vector<glm::mat3x4>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 12, (int32_t)vec.size() };
			break;
		}
		case MaterialParamaterType::Matrix4x3Vector: {
			uniformType = UniformType::Matrix4x3Vector;
			auto& vec = std::get<std::vector<glm::mat4x3>>(data);
			uniformData.FloatVector = UniformData::FloatVectorType{ (float*)vec.data(), 12, (int32_t)vec.size() };
			break;
		}

		case MaterialParamaterType::Sampler2D: {
			uniformType = UniformType::Int1;
			uniformData.Int1 = bindingA;
			std::get<Texture2DRef>(data)->Bind(bindingA++, bindingB);
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

}