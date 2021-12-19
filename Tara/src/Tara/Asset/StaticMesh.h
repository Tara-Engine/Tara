#pragma once
#include "Tara/Asset/Asset.h"
#include "Tara/Renderer/VertexArray.h"
#include "Tara/Utility/MeshPart.h"


namespace Tara {
	REFTYPE(StaticMesh);




	/// <summary>
	/// The StaticMesh asset is for holding and managing a static 3D mesh, 
	/// for rendering, physics, and all other uses.
	/// </summary>
	class StaticMesh : public Asset {
	public:
		/// <summary>
		/// Construct a StaticMesh from a list of MeshParts. Should not be called manually.
		/// </summary>
		/// <param name="parts">the list of mesh parts</param>
		/// <param name="name">the asset name</param>
		StaticMesh(const std::initializer_list<MeshPart>& parts, const std::string& name);

		/// <summary>
		/// Construct a StaticMesh from a list of VertexArrayRefs. Should not be called manually.
		/// </summary>
		/// <param name="parts">list of mesh parts as VertexArrayRef objects. Does not confirm the layout.</param>
		/// <param name="name">the asset name</param>
		StaticMesh(const std::initializer_list<VertexArrayRef>& parts, const std::string& name);



		/// <summary>
		/// Create a StaticMesh from a list of MeshParts.
		/// </summary>
		/// <param name="parts">the list of mesh parts</param>
		/// <param name="name">the asset name</param>
		/// <returns></returns>
		static StaticMeshRef Create(const std::initializer_list<MeshPart>& parts, const std::string& name);
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="parts">list of mesh parts as VertexArrayRef objects. Does not confirm the layout.</param>
		/// <param name="name">the asset name</param>
		/// <returns></returns>
		static StaticMeshRef Create(const std::initializer_list<VertexArrayRef>& parts, const std::string& name);



		virtual ~StaticMesh() = default;

		/// <summary>
		/// Return the vertex arrays
		/// </summary>
		/// <returns></returns>
		const std::vector<VertexArrayRef>& GetVertexArrays() const { return m_VertexArrays; }

	private:
		std::vector<VertexArrayRef> m_VertexArrays;

	};



}