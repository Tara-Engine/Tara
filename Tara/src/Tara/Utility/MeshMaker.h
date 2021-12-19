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
		enum class Mode {
			TRIANGLES, TRIANGLE_STRIP, TRIANGLE_FAN, QUADS
		};
	
	public:
		MeshMaker(Mode mode);

		void Vertex(Vector pos);

		inline void Vertex(float x, float y, float z) { Vertex({ x,y,z }); }

		void Normal(Vector norm);

		inline void Normal(float x, float y, float z) { Normal({ x,y,z }); }

		void Color(glm::vec4 color);

		inline void Color(float r, float g, float b, float a) { Color({ r,g,b,a }); }

		void TextureCoord(glm::vec2 coords);

		void TextureCoord(float u, float v) { TextureCoord({ u,v }); }

		//inline void SetSmoothAngle(float smoothAngle) {m_SmoothAngle = smoothAngle;}

		void PushPrimitive();

		void DumpPrimitive();

		MeshPart GetMeshPart();

	private:
		std::vector<Tara::Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		Mode m_CurrentMode;
		Tara::Vertex m_WorkingPrimitive[4];
		int32_t m_IndexCache[2];
		int32_t m_WorkingIndex;
		bool m_Alternator;
		//float m_SmoothAngle;
	};


}