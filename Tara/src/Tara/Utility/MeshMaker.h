#pragma once
#include "Tara/Utility/MeshPart.h"

namespace Tara {

	/// <summary>
	/// The MeshMaker is a utility for generating procedural MeshParts.
	/// One is created with a Mode, then calls to Vertex(), Normal(), Color(), and TextureCoord() 
	/// are made to set these values for the working vertex (updated by a call to Vertex())
	/// Each mode has a different manner of pushing a triangle to the list of finalized trigs. PushPrimitive() will push 
	/// a primitive if available, while DumpPrivitive() will dump the working primitive 
	/// (effetively restarting the mesh without clearing previously pushed primitives)
	/// When complete, GetMeshPart() will get a mesh part.
	/// </summary>
	class MeshMaker {
	public:
		/// <summary>
		/// Mode for the MeshMaker. This controls how the passed vertecies combine into triangles
		/// </summary>
		enum class Mode {
			/// <summary>
			/// Each three vertices become one triangle (Clockwise ordering is front)
			/// </summary>
			TRIANGLES, 
			/// <summary>
			/// The first three vertices become the first triangle, then the last two of the previous triangle plus the next vertex become the next triangle.
			/// </summary>
			TRIANGLE_STRIP, 
			/// <summary>
			/// The first vertex passed is the center, and then each vertex (starting with next two) around it make triangles with the center, the previous vertex, and itself
			/// </summary>
			TRIANGLE_FAN, 
			/// <summary>
			/// Each four fertices become one quad (2 triangles) (Clockwise ordering is front)
			/// </summary>
			QUADS
		};
		/// <summary>
		/// Rules for how vertices should be combined
		/// </summary>
		enum class CombineRules {
			/// <summary>
			/// Never combine vertices
			/// </summary>
			NEVER, 
			/// <summary>
			/// Combine if the second is default in its non-positional values
			/// </summary>
			IF_SECOND_DEFAULT, 
			/// <summary>
			/// Combine if they match, but ignoring the normal
			/// </summary>
			IF_MATCH_IGNORE_NORMAL, 
			/// <summary>
			/// Combine if all fields match
			/// </summary>
			IF_MATCH_ALL 
		};
	
	public:
		/// <summary>
		/// Construct a new MeshMaker
		/// </summary>
		/// <param name="mode">the starting mode</param>
		MeshMaker(Mode mode);

		/// <summary>
		/// Specify a new vertex by position. This becomes the current working vertex. (will automatically push the current primitive.)
		/// </summary>
		/// <param name="pos"></param>
		void Vertex(Vector pos);

		/// <summary>
		/// Specify a new vertex by position. This becomes the current working vertex. (will automatically push the current primitive.)
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="z"></param>
		inline void Vertex(float x, float y, float z) { Vertex({ x,y,z }); }

		/// <summary>
		/// Specify the normal for the current working vertex.
		/// </summary>
		/// <param name="norm"></param>
		void Normal(Vector norm);

		/// <summary>
		/// Specify the normal for the current working vertex.
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="z"></param>
		inline void Normal(float x, float y, float z) { Normal({ x,y,z }); }

		/// <summary>
		/// Specify the color for the current working vertex.
		/// </summary>
		/// <param name="color"></param>
		void Color(glm::vec4 color);

		/// <summary>
		/// Specify the color for the current working vertex.
		/// </summary>
		/// <param name="r"></param>
		/// <param name="g"></param>
		/// <param name="b"></param>
		/// <param name="a"></param>
		inline void Color(float r, float g, float b, float a) { Color({ r,g,b,a }); }

		/// <summary>
		/// Specify the UV coordinates for the current working vertex.
		/// </summary>
		/// <param name="coords"></param>
		void TextureCoord(glm::vec2 coords);

		/// <summary>
		/// Specify the UV coordinates for the current working vertex.
		/// </summary>
		/// <param name="u"></param>
		/// <param name="v"></param>
		void TextureCoord(float u, float v) { TextureCoord({ u,v }); }

		/// <summary>
		/// Set the combine rule for all new primitives.
		/// </summary>
		/// <param name="rule"></param>
		inline void SetCombineRule(CombineRules rule) { PushPrimitive(); m_CurrentCombine = rule; }

		/// <summary>
		/// Specify the Mode for all new primitives. Incomplete primitives are dumped.
		/// </summary>
		/// <param name="mode"></param>
		inline void SetMode(Mode mode) { PushPrimitive(); m_CurrentMode = mode; DumpPrimitive(); }

		/// <summary>
		/// Ensure that the current vertex and its primitive are pushed. May reset the current working vertex to a previous vertex, or to no vertex.
		/// </summary>
		void PushPrimitive();

		/// <summary>
		/// Dumps any partial or unpushed primitives. 
		/// (ie, if mode is TRIANGLES and only three vertices have been specified, 
		/// they will be dumped, as they are not pushed till a call to PushPrimitive or Vertex)
		/// </summary>
		void DumpPrimitive();

		/// <summary>
		/// Get a MeshPart from the current pushed state (will automatically push the current primitive.)
		/// </summary>
		/// <returns></returns>
		MeshPart GetMeshPart();

	private:
		/// <summary>
		/// Add a new vertex, respecting combining rules. Internal use only
		/// ONLY returns the index to use
		/// </summary>
		/// <param name="vertex"></param>
		/// <returns></returns>
		uint32_t AddNewVertex(const Tara::Vertex& vertex);

		/// <summary>
		/// Add a new triangle, from the indices of its verts
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <param name="c"></param>
		void AddNewTriangle(uint32_t a, uint32_t b, uint32_t c);

	private:
		std::vector<Tara::Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		Mode m_CurrentMode;
		CombineRules m_CurrentCombine;
		Tara::Vertex m_WorkingPrimitive[4];
		int32_t m_IndexCache[2];
		int32_t m_WorkingIndex;
		bool m_Alternator;
		
	};


}