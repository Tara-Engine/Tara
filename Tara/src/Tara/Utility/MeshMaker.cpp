#include "tarapch.h"
#include "MeshMaker.h"

namespace Tara{
	MeshMaker::MeshMaker(Mode mode)
		: m_CurrentMode(mode), m_WorkingIndex(-1), m_Alternator(false)
	{
		m_WorkingPrimitive[0] = { 0,0,0 };
		m_WorkingPrimitive[1] = { 0,0,0 };
		m_WorkingPrimitive[2] = { 0,0,0 };
		m_WorkingPrimitive[3] = { 0,0,0 };
		m_IndexCache[0] = -1;
		m_IndexCache[1] = -1;
	}

	void MeshMaker::Vertex(Vector pos)
	{
		PushPrimitive(); //always push first, in case last vertex was final of that primitive

		m_WorkingIndex++; //move to the next working index
		m_WorkingPrimitive[m_WorkingIndex].Position = pos;
		m_WorkingPrimitive[m_WorkingIndex].Normal = { 0,0,0 };
		m_WorkingPrimitive[m_WorkingIndex].Color = { 1,1,1,1 };
		m_WorkingPrimitive[m_WorkingIndex].UV = { 1,1 };
	}

	void MeshMaker::Normal(Vector norm)
	{
		if (m_WorkingIndex > -1 && m_WorkingIndex < 4) {
			m_WorkingPrimitive[m_WorkingIndex].Normal = norm;
		}
		else {
			LOG_S(WARNING) << "Attemtping to call MeshMaker::Normal without a valid working Vertex";
		}
	}

	void MeshMaker::Color(glm::vec4 color)
	{
		if (m_WorkingIndex > -1 && m_WorkingIndex < 4) {
			m_WorkingPrimitive[m_WorkingIndex].Color = color;
		}
		else {
			LOG_S(WARNING) << "Attemtping to call MeshMaker::Color without a valid working Vertex";
		}
	}

	void MeshMaker::TextureCoord(glm::vec2 coords)
	{
		if (m_WorkingIndex > -1 && m_WorkingIndex < 4) {
			m_WorkingPrimitive[m_WorkingIndex].UV = coords;
		}
		else {
			LOG_S(WARNING) << "Attemtping to call MeshMaker::TextureCoord without a valid working Vertex";
		}
	}

	void MeshMaker::PushPrimitive()
	{
		if (m_WorkingIndex < -1 || m_WorkingIndex > 3) {
			LOG_S(WARNING) << "Attemtping to call MeshMaker::PushPrimitive without a valid working Vertex";
			return;
		}

		switch (m_CurrentMode) {

		case Mode::TRIANGLES : {
			if (m_WorkingIndex >= 2) {
				//should *only* be 2.
				
				//move the three triangles over.
				for (int i = 0; i < 3; i++) {
					m_Vertices.push_back(m_WorkingPrimitive[i]);
					m_Indices.push_back(m_Vertices.size() - 1);
				}

				m_WorkingIndex = -1;
			}

			break;
		}

		case Mode::TRIANGLE_FAN: {

			if (m_WorkingIndex >= 2) {
				//should *only* be 2.

				if (m_IndexCache[0] > -1 && m_IndexCache[1] > -1) {
					//if the index cache is valid, then use those indecies. update the second cached with the current third
					m_Vertices.push_back(m_WorkingPrimitive[2]);
					m_Indices.push_back(m_IndexCache[0]);
					m_Indices.push_back(m_IndexCache[1]);
					m_Indices.push_back(m_Vertices.size() - 1);
					m_IndexCache[1] = m_Vertices.size() - 1;
				}
				else {
					//if the index cache is not valid, its the first bit of the strip, add all three and cache the first and third indecies
					m_Vertices.push_back(m_WorkingPrimitive[0]);
					m_IndexCache[0] = m_Vertices.size() - 1;
					m_Indices.push_back(m_Vertices.size() - 1);

					m_Vertices.push_back(m_WorkingPrimitive[1]);
					m_Indices.push_back(m_Vertices.size() - 1);
					
					m_Vertices.push_back(m_WorkingPrimitive[2]);
					m_IndexCache[1] = m_Vertices.size() - 1;
					m_Indices.push_back(m_Vertices.size() - 1);
				}

				//decrease by one (IE, reset to the second primitive.)
				m_WorkingIndex--;
			}
			break;
		}

		case Mode::TRIANGLE_STRIP: {

			if (m_WorkingIndex >= 2) {
				//should *only* be 2.

				if (m_IndexCache[0] > -1 && m_IndexCache[1] > -1) {
					//if the index cache is valid, then use those indecies. slide the cache down and update it with current third
					m_Vertices.push_back(m_WorkingPrimitive[2]);
					m_Indices.push_back(m_IndexCache[0]);
					m_Indices.push_back(m_IndexCache[1]);
					m_Indices.push_back(m_Vertices.size() - 1);
					m_IndexCache[0] = m_IndexCache[1];
					m_IndexCache[1] = m_Vertices.size() - 1;
				}
				else {
					//if the index cache is not valid, its the first bit of the strip, add all three and cache the second and third indecies
					m_Vertices.push_back(m_WorkingPrimitive[0]);
					m_Indices.push_back(m_Vertices.size() - 1);
					
					m_Vertices.push_back(m_WorkingPrimitive[1]);
					m_IndexCache[0] = m_Vertices.size() - 1;
					m_Indices.push_back(m_Vertices.size() - 1);
					
					m_Vertices.push_back(m_WorkingPrimitive[2]);
					m_IndexCache[1] = m_Vertices.size() - 1;
					m_Indices.push_back(m_Vertices.size() - 1);
				}

				//decrease by one (IE, reset to the second primitive.)
				if (m_Alternator) {
					int l = m_Indices.size() - 1;
					std::swap(m_Indices[l], m_Indices[l - 1]);
				}
				m_WorkingIndex--;
				m_Alternator = !m_Alternator;
			}
			break;
		}

		case Mode::QUADS: {
			if (m_WorkingIndex >= 3) {
				//should ever only be 3

				m_Vertices.push_back(m_WorkingPrimitive[0]);
				m_Indices.push_back(m_Vertices.size() - 1);
				uint32_t firstIndex = m_Vertices.size() - 1;

				m_Vertices.push_back(m_WorkingPrimitive[1]);
				m_Indices.push_back(m_Vertices.size() - 1);

				m_Vertices.push_back(m_WorkingPrimitive[2]);
				m_Indices.push_back(m_Vertices.size() - 1);

				m_Indices.push_back(m_Vertices.size() - 1);

				m_Vertices.push_back(m_WorkingPrimitive[3]);
				m_Indices.push_back(m_Vertices.size() - 1);

				m_Indices.push_back(firstIndex);

				m_WorkingIndex = -1;
			}
			break;
		}

		default: {
			LOG_S(WARNING) << "Unknown Mode, dumping current primitive and changing mode to TRIANGLES";
			m_CurrentMode = Mode::TRIANGLES;
			m_WorkingIndex = -1;
		}

		}

	}

	void MeshMaker::DumpPrimitive()
	{
	}

	MeshPart MeshMaker::GetMeshPart()
	{
		PushPrimitive();
		return MeshPart(m_Vertices, m_Indices);
	}


}
