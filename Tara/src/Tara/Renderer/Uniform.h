#pragma once
#include <glm/glm.hpp>


namespace Tara {

	/// <summary>
	/// A standard format for sending data to a shader's uniform
	/// </summary>
	union UniformData {
		float Float1;
		glm::vec2 Float2;
		glm::vec3 Float3;
		glm::vec4 Float4;

		int32_t Int1;
		glm::ivec2 Int2;
		glm::ivec3 Int3;
		glm::ivec4 Int4;

		uint32_t UInt1;
		glm::uvec2 UInt2;
		glm::uvec3 UInt3;
		glm::uvec4 UInt4;

		//ALL vector types must have its pointer first

		struct FloatVectorType {
			char _spacer[8];
			int32_t Count;
			int32_t Elements;
			float* Data;
			FloatVectorType();
			FloatVectorType(const float* data, int32_t Count, int32_t Elements);
			FloatVectorType(const FloatVectorType& other);
			FloatVectorType(FloatVectorType&& other);
			FloatVectorType& operator=(const FloatVectorType& other);
			FloatVectorType& operator=(FloatVectorType&& other);
			~FloatVectorType();
		};
		FloatVectorType FloatVector;

		struct IntVectorType {
			char _spacer[8];
			int32_t Count;
			int32_t Elements;
			int32_t* Data;
			IntVectorType();
			IntVectorType(const int32_t* data, int32_t Count, int32_t Elements);
			IntVectorType(const IntVectorType& other);
			IntVectorType(IntVectorType&& other);
			IntVectorType& operator=(const IntVectorType& other);
			IntVectorType& operator=(IntVectorType&& other);
			~IntVectorType();
		};
		IntVectorType IntVector;

		struct UIntVectorType {
			char _spacer[8];
			int32_t Count;
			int32_t Elements;
			uint32_t* Data;
			UIntVectorType();
			UIntVectorType(const uint32_t* data, int32_t Count, int32_t Elements);
			UIntVectorType(const UIntVectorType& other);
			UIntVectorType(UIntVectorType&& other);
			UIntVectorType& operator=(const UIntVectorType& other);
			UIntVectorType& operator=(UIntVectorType&& other);
			~UIntVectorType();
		};

		UIntVectorType UIntVector;

		UniformData(float);
		~UniformData();
	};


	enum class UniformType {
		Float1, Float2, Float3, Float4,
		Int1, Int2, Int3, Int4,
		UInt1, UInt2, UInt3, UInt4,
		Float1Vector, Float2Vector, Float3Vector, Float4Vector,
		Int1Vector, Int2Vector, Int3Vector, Int4Vector,
		UInt1Vector, UInt2Vector, UInt3Vector, UInt4Vector,
		Matrix2Vector, Matrix3Vector, Matrix4Vector,
		Matrix2x3Vector, Matrix3x2Vector,
		Matrix2x4Vector, Matrix4x2Vector,
		Matrix3x4Vector, Matrix4x3Vector
	};

	struct Uniform {
		UniformType DataType;
		UniformData Data;
		Uniform(UniformType type, const UniformData& data);
		Uniform(UniformType type, UniformData&& data);
		Uniform(const Uniform& other);
		Uniform(Uniform&& other);
		Uniform& operator=(const Uniform& other);
		Uniform& operator=(Uniform&& other);
		~Uniform();
	private:
		void SetData(const UniformData& data);
		void SetData(UniformData&& data);
		void ClearOldData();
	public:
		static uint32_t S_Allocations;
	};
}