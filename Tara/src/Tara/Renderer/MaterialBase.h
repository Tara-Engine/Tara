#pragma once
#include "Tara/Renderer/Texture.h"
#include "Tara/Renderer/Uniform.h"

namespace Tara {
	REFTYPE(MaterialBase);
	REFTYPE(MaterialInstance);

	enum class MaterialParamaterType {
		INVALID, //for Invalid types
		Float1, Float2, Float3, Float4,
		Int1, Int2, Int3, Int4,
		UInt1, UInt2, UInt3, UInt4,
		Float1Vector, Float2Vector, Float3Vector, Float4Vector,
		Int1Vector, Int2Vector, Int3Vector, Int4Vector,
		UInt1Vector, UInt2Vector, UInt3Vector, UInt4Vector,
		Matrix2Vector, Matrix3Vector, Matrix4Vector,
		Matrix2x3Vector, Matrix3x2Vector,
		Matrix2x4Vector, Matrix4x2Vector,
		Matrix3x4Vector, Matrix4x3Vector,
		Sampler1D, Sampler2D, Sampler3D, SamplerCube, Sampler1DArray, Sampler2DArray,
		Image1D, Image2D, Image3D, ImageCube, Image1DArray, Image2DArray
	};

	using MaterialParamater = std::variant<
		float, glm::vec2, glm::vec3, glm::vec4,
		int32_t, glm::ivec2, glm::ivec3, glm::ivec4,
		uint32_t, glm::uvec2, glm::uvec3, glm::uvec4,

		std::vector<float>, std::vector<glm::vec2>, std::vector<glm::vec3>, std::vector<glm::vec4>,
		std::vector<int32_t>, std::vector<glm::ivec2>, std::vector<glm::ivec3>, std::vector<glm::ivec4>,
		std::vector<uint32_t>, std::vector<glm::uvec2>, std::vector<glm::uvec3>, std::vector<glm::uvec4>,

		std::vector<glm::mat2>, std::vector <glm::mat3>, std::vector <glm::mat4>,
		std::vector<glm::mat2x3>, std::vector<glm::mat3x2>,
		std::vector<glm::mat2x4>, std::vector<glm::mat4x2>,
		std::vector<glm::mat3x4>, std::vector<glm::mat4x3>,
		Texture2DRef,
		TextureCubemapRef
	>;

	/// <summary>
	/// The type of material. UNLIT is rendered in a Forward manner, while LIT is rendered in a Deferred manner. LIGHTING is used to calculate lighting. POSTPROCESS is used after lighting.
	/// </summary>
	enum class MaterialType {
		LIGHTING = 0b0000,
		UNLIT = 0b0001, 
		LIT = 0b0010, 
		POSTPROCESS = 0b0100,
	};

	class MaterialBase : public std::enable_shared_from_this<MaterialBase> {
	public:
		/// <summary>
		/// Get the MaterialParamaterType from a MaterialParamater variable.
		/// </summary>
		/// <param name="data"></param>
		/// <returns></returns>
		static MaterialParamaterType MaterialParamaterTypeFromData(const MaterialParamater& data);

		/// <summary>
		/// returns true if the data and the type match
		/// </summary>
		/// <param name="data"></param>
		/// <param name="type"></param>
		/// <returns></returns>
		static bool MaterialParamaterDataAndTypeMatch(const MaterialParamater& data, MaterialParamaterType type);

		/// <summary>
		/// Make a uniform from a material paramater. Does not check to see if the data and the type match, this is assumed.
		/// Any Texture paramaters will be bound automatically, using the provided binding indices. bindingA will be automatically incremented if used, while bindingB is left alone.
		/// </summary>
		/// <param name="data">the paramater data</param>
		/// <param name="type">the paramater type</param>
		/// <param name="bindingA">the location to bind any texture to (will change)</param>
		/// <param name="bindingB">the selector for binding (will not change)</param>
		/// <returns></returns>
		static Uniform MaterialParamaterToUniform(const MaterialParamater& data, MaterialParamaterType type, int32_t& bindingA, int32_t bindingB);

		/// <summary>
		/// Get a material paramater type from the glsl string of that type
		/// </summary>
		/// <param name="type">the glsl type name</param>
		/// <param name="isVector">if its a vector</param>
		/// <returns>the MaterialParamaterType</returns>
		static MaterialParamaterType MaterialParamaterTypeFromString(const std::string type, bool isVector);

		/// <summary>
		/// Get the default MaterialParamater value for any given type
		/// </summary>
		/// <param name="type"></param>
		/// <returns></returns>
		static MaterialParamater DefaultMaterialParamaterFromType(MaterialParamaterType type);
	public:

		/// <summary>
		/// Use this material. Binds the underlying shader and sets up the uniforms.
		/// </summary>
		virtual void Use() = 0;

		/// <summary>
		/// Get the material types
		/// </summary>
		/// <returns></returns>
		virtual MaterialType GetType() = 0;

		/// <summary>
		/// Get the shader being used.
		/// </summary>
		/// <returns></returns>
		virtual const ShaderRef& GetShader() = 0;

		/// <summary>
		/// Get a paramater list
		/// </summary>
		/// <returns></returns>
		virtual std::vector<std::pair<std::string, MaterialParamaterType>> GetParamaterList() = 0;

		/// <summary>
		/// Get the type of the paramater. If the name is not a valid paramater, then a warning will be printed, and Float1 returned.
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		virtual MaterialParamaterType GetParamaterType(const std::string& name) = 0;

		/// <summary>
		/// Check if a paramater name is a valid paramater
		/// </summary>
		/// <param name="name">the paramater name</param>
		/// <returns></returns>
		virtual bool GetParamaterValid(const std::string& name) = 0;

		/// <summary>
		/// Get the current value of a paramater
		/// </summary>
		/// <param name="name">the name of the paramamter</param>
		/// <returns>pair of the type and the value</returns>
		virtual std::pair<MaterialParamaterType, MaterialParamater> GetParamaterValue(const std::string& name) = 0;

		/// <summary>
		/// Base setting of a paramater
		/// </summary>
		/// <param name="name">the paramater name</param>
		/// <param name="value">the paramater value</param>
		virtual void SetParamater(const std::string& name, MaterialParamater value) = 0;

		/// <summary>
		/// Create a new MaterialInstance from this Material
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		virtual MaterialInstanceRef CreateInstance(const std::string name);

		/// <summary>
		/// Create a new MaterialInstance from this Material. 
		/// The new instance is not registered with the Asset Library. 
		/// Useful for temporary Instances
		/// </summary>
		/// <returns></returns>
		virtual MaterialInstanceRef CreateInstanceUnregistered();

		inline virtual void SetScalarParamater(const std::string& name, float value) { SetParamater(name, value); }
		inline virtual void SetScalarParamater(const std::string& name, int32_t value) { SetParamater(name, value); }
		inline virtual void SetScalarParamater(const std::string& name, uint32_t value) { SetParamater(name, value); }

		inline virtual void SetVector2Paramater(const std::string& name, glm::vec2  value) { SetParamater(name, value); }
		inline virtual void SetVector2Paramater(const std::string& name, glm::ivec2 value) { SetParamater(name, value); }
		inline virtual void SetVector2Paramater(const std::string& name, glm::uvec2 value) { SetParamater(name, value); }

		inline virtual void SetVector3Paramater(const std::string& name, glm::vec3  value) { SetParamater(name, value); }
		inline virtual void SetVector3Paramater(const std::string& name, glm::ivec3 value) { SetParamater(name, value); }
		inline virtual void SetVector3Paramater(const std::string& name, glm::uvec3 value) { SetParamater(name, value); }

		inline virtual void SetVector4Paramater(const std::string& name, glm::vec4  value) { SetParamater(name, value); }
		inline virtual void SetVector4Paramater(const std::string& name, glm::ivec4 value) { SetParamater(name, value); }
		inline virtual void SetVector4Paramater(const std::string& name, glm::uvec4 value) { SetParamater(name, value); }

		inline virtual void SetScalarArrayParamater(const std::string& name, const std::vector<float>& value) { SetParamater(name, value); }
		inline virtual void SetScalarArrayParamater(const std::string& name, const std::vector<int32_t>& value) { SetParamater(name, value); }
		inline virtual void SetScalarArrayParamater(const std::string& name, const std::vector<uint32_t>& value) { SetParamater(name, value); }

		inline virtual void SetVector2ArrayParamater(const std::string& name, const std::vector<glm::vec2>& value) { SetParamater(name, value); }
		inline virtual void SetVector2ArrayParamater(const std::string& name, const std::vector<glm::ivec2>& value) { SetParamater(name, value); }
		inline virtual void SetVector2ArrayParamater(const std::string& name, const std::vector<glm::uvec2>& value) { SetParamater(name, value); }

		inline virtual void SetVector3ArrayParamater(const std::string& name, const std::vector<glm::vec3>& value) { SetParamater(name, value); }
		inline virtual void SetVector3ArrayParamater(const std::string& name, const std::vector<glm::ivec3>& value) { SetParamater(name, value); }
		inline virtual void SetVector3ArrayParamater(const std::string& name, const std::vector<glm::uvec3>& value) { SetParamater(name, value); }

		inline virtual void SetVector4ArrayParamater(const std::string& name, const std::vector<glm::vec4>& value) { SetParamater(name, value); }
		inline virtual void SetVector4ArrayParamater(const std::string& name, const std::vector<glm::ivec4>& value) { SetParamater(name, value); }
		inline virtual void SetVector4ArrayParamater(const std::string& name, const std::vector<glm::uvec4>& value) { SetParamater(name, value); }

		inline virtual void SetMatrixParmamter(const std::string& name, glm::mat2 value) { SetParamater(name, std::vector<glm::mat2>({ value })); }
		inline virtual void SetMatrixParmamter(const std::string& name, glm::mat3 value) { SetParamater(name, std::vector<glm::mat3>({ value })); }
		inline virtual void SetMatrixParmamter(const std::string& name, glm::mat4 value) { SetParamater(name, std::vector<glm::mat4>({ value })); }

		inline virtual void SetRectangularMatrixParmamter(const std::string& name, glm::mat2x3 value) { SetParamater(name, std::vector<glm::mat2x3>({ value })); }
		inline virtual void SetRectangularMatrixParmamter(const std::string& name, glm::mat3x2 value) { SetParamater(name, std::vector<glm::mat3x2>({ value })); }
		inline virtual void SetRectangularMatrixParmamter(const std::string& name, glm::mat2x4 value) { SetParamater(name, std::vector<glm::mat2x4>({ value })); }
		inline virtual void SetRectangularMatrixParmamter(const std::string& name, glm::mat4x2 value) { SetParamater(name, std::vector<glm::mat4x2>({ value })); }
		inline virtual void SetRectangularMatrixParmamter(const std::string& name, glm::mat3x4 value) { SetParamater(name, std::vector<glm::mat3x4>({ value })); }
		inline virtual void SetRectangularMatrixParmamter(const std::string& name, glm::mat4x3 value) { SetParamater(name, std::vector<glm::mat4x3>({ value })); }

		inline virtual void SetMatrixArrayParmamter(const std::string& name, const std::vector<glm::mat2>& value) { SetParamater(name, value); }
		inline virtual void SetMatrixArrayParmamter(const std::string& name, const std::vector<glm::mat3>& value) { SetParamater(name, value); }
		inline virtual void SetMatrixArrayParmamter(const std::string& name, const std::vector<glm::mat4>& value) { SetParamater(name, value); }

		inline virtual void SetRectangularMatrixArrayParmamter(const std::string& name, const std::vector<glm::mat2x3>& value) { SetParamater(name, value); }
		inline virtual void SetRectangularMatrixArrayParmamter(const std::string& name, const std::vector<glm::mat3x2>& value) { SetParamater(name, value); }
		inline virtual void SetRectangularMatrixArrayParmamter(const std::string& name, const std::vector<glm::mat2x4>& value) { SetParamater(name, value); }
		inline virtual void SetRectangularMatrixArrayParmamter(const std::string& name, const std::vector<glm::mat4x2>& value) { SetParamater(name, value); }
		inline virtual void SetRectangularMatrixArrayParmamter(const std::string& name, const std::vector<glm::mat3x4>& value) { SetParamater(name, value); }
		inline virtual void SetRectangularMatrixArrayParmamter(const std::string& name, const std::vector<glm::mat4x3>& value) { SetParamater(name, value); }

		inline virtual void SetTextureParmamter(const std::string& name, Texture2DRef value) { SetParamater(name, value); }

	};
}