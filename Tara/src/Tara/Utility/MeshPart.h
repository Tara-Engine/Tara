#pragma once
#include "Tara/Math/Types.h"
#include "Tara/Renderer/VertexArray.h"

namespace Tara {


	/// <summary>
	/// A basic Vertex for a mesh
	/// </summary>
	struct Vertex {
		Vector Position;
		Vector Normal;
		Vector Tangent;
		glm::vec4 Color;
		glm::vec2 UV;

		/// <summary>
		/// Construct a new Vertex, from parts nicly encapsulated
		/// </summary>
		/// <param name="pos">the Position of the vertex</param>
		/// <param name="norm">the Normal of the vertex, defaults to {0,0,0}</param>
		/// <param name="color">the Color of the vertex, defaults to {1,1,1,1} (Opaque White)</param>
		/// <param name="uv">the UV coordinate of the vertex, defaults to {0,0}</param>
		Vertex(Vector pos, Vector norm = { 0,1,0 }, glm::vec4 color = { 1,1,1,1 }, glm::vec2 uv = { 0,0 })
			:Position(pos), Normal(norm), Tangent(0), Color(color), UV(uv)
		{}

		/// <summary>
		/// Construct a new Vertex, from raw floats
		/// </summary>
		/// <param name="x">X position, defaults to 0</param>
		/// <param name="y">Y position, defaults to 0</param>
		/// <param name="z">Z position, defaults to 0</param>
		/// <param name="nx">X normal, defaults to 0</param>
		/// <param name="ny">Y normal, defaults to 0</param>
		/// <param name="nz">Z normal, defaults to 0</param>
		/// <param name="r">Red element of the Color, defaults to 1</param>
		/// <param name="g">Green element of the Color, defaults to 1</param>
		/// <param name="b">Blue element of the Color, defaults to 1</param>
		/// <param name="a">Alpha(transparency) element of the Color, defaults to 1</param>
		/// <param name="u">U texture coordinate, defaults to 0</param>
		/// <param name="v">V texture coordinate, defaults to 0</param>
		Vertex(float x = 0, float y = 0, float z = 0, float nx = 0, float ny = 0, float nz = 0, float r = 1, float g = 1, float b = 1, float a = 1, float u = 0, float v = 0)
			:Position(x, y, z), Normal(nx, ny, nz), Tangent(0), Color(r, g, b, a), UV(u, v)
		{}

	};
	
	/// <summary>
	/// The number of floats in a Vertex
	/// </summary>
	constexpr uint64_t VERTEX_FLOATS = sizeof(Vertex) / sizeof(float);

	const BufferLayout VERTEX_LAYOUT = BufferLayout({
		{Tara::Shader::Datatype::Float3, "Position", false},
		{Tara::Shader::Datatype::Float3, "Normal", false},
		{Tara::Shader::Datatype::Float3, "Tangent", false},
		{Tara::Shader::Datatype::Float4, "Color", false},
		{Tara::Shader::Datatype::Float2, "UV", false},
	});

	/// <summary>
	/// The Mesh part is a mutable segment of a mesh
	/// </summary>
	struct MeshPart {
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;

		/// <summary>
		/// Default Constructor
		/// </summary>
		MeshPart() = default;

		/// <summary>
		/// Construct from vectors
		/// </summary>
		/// <param name="vertices">the vertex list</param>
		/// <param name="indices">the index list</param>
		MeshPart(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
			: Vertices(vertices), Indices(indices)
		{}

		/// <summary>
		/// Construct from vectors
		/// </summary>
		/// <param name="vertices">the vertex list</param>
		/// <param name="indices">the index list</param>
		MeshPart(std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices)
			: Vertices(std::move(vertices)), Indices(std::move(indices))
		{}

		/// <summary>
		/// Explicit Copy Constructor
		/// </summary>
		/// <param name="other"></param>
		MeshPart(const MeshPart& other) = default;

		/// <summary>
		/// Explicit move constructor
		/// </summary>
		/// <param name="other"></param>
		MeshPart(MeshPart&& other) = default;

		/// <summary>
		/// Explicit copy assignment operator
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		MeshPart& operator=(const MeshPart& other) = default;

		/// <summary>
		/// Explicit move assignment operator
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		MeshPart& operator=(MeshPart&& other) = default;

	public:
		//modifying fucntions. These work in-place

		/// <summary>
		/// Calculate the normals for the Mesh part.
		/// Normals are always assumed to be smooth, to have sharp normals, use seperate vertecies on the same location
		/// This is a modifying operation, changing the object.
		/// </summary>
		/// <returns>A reference to itself</returns>
		MeshPart& CalculateNormals();
		
		/// <summary>
		/// Flip the normals for the Mesh part
		/// </summary>
		/// <returns>A reference to itself </returns>
		MeshPart& FlipNormals();

		/// <summary>
		/// Flip the order (clockwise/counterclockwise) that the vertecies of the triangels are ordered in.
		/// </summary>
		/// <returns>A reference to itself </returns>
		MeshPart& FlipWindings();

		/// <summary>
		/// Flip both the normals and the Windings in one function call. Inverts the mesh
		/// </summary>
		/// <returns>A reference to itself </returns>
		inline MeshPart& FlipTriangles() { FlipNormals(); FlipWindings(); return *this; }

		/// <summary>
		/// Delete all normals
		/// </summary>
		/// <returns>A reference to itself </returns>
		MeshPart& ClearNormals();

		/// <summary>
		/// Using the existing normals and UVs, calculate the Tangent and Bitangent for the mesh
		/// A division by 0 error is likely if you have not set up the mesh's UVs
		/// </summary>
		/// <returns></returns>
		MeshPart& CalculateTangents();

		/// <summary>
		/// Transform the vertices positions by a matrix
		/// </summary>
		/// <param name="matrix">the matrix to transform the vertices by</param>
		/// <returns>A reference to itself</returns>
		MeshPart& Transform(glm::mat4 matrix);

	public:
		//Combination operations (do not work in-place)


	public:
		//Basic Primitive Generation functions

		/// <summary>
		/// Create a unit cube mesh, with normals, colors, and UVs
		/// </summary>
		/// <returns></returns>
		static MeshPart UnitCube();

		/// <summary>
		/// Create a Sphere. UVs are not provided. Needs more work, there are also normal issues
		/// </summary>
		/// <returns></returns>
		static MeshPart UnitSphere(int32_t strips_v = 16, int32_t strips_h = 32);
	public:
		//other utility

		/// <summary>
		/// Convert the Meshpart to a VertexArray
		/// </summary>
		/// <returns></returns>
		VertexArrayRef ToVertexArray() const;
	};


}