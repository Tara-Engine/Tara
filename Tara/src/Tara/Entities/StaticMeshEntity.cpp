#include "tarapch.h"
#include "StaticMeshEntity.h"
#include "Tara/Renderer/Renderer.h"

namespace Tara{
	StaticMeshEntity::StaticMeshEntity(EntityNoRef parent, LayerNoRef owningLayer, Transform transform, StaticMeshRef mesh, MaterialBaseRef material, const std::string& name)
		: Entity(parent, owningLayer, transform, name), m_StaticMesh(mesh), m_Materials(mesh->GetArrayCount(), material)
	{}

	void StaticMeshEntity::OnDraw(float deltaTime)
	{
		for (int i = 0; i < m_StaticMesh->GetArrayCount();i++) {
			m_Materials[i]->Use();
			Renderer::Draw(m_StaticMesh->GetVertexArrays()[i], m_Materials[i]->GetShader(), m_Transform);
		}
	}

	void StaticMeshEntity::SetMaterial(int materialID, MaterialBaseRef material)
	{
		if (materialID < m_StaticMesh->GetArrayCount()) {
			m_Materials[materialID] = material;
		}
	}

	const MaterialBaseRef& StaticMeshEntity::GetMaterial(int materialID) const
	{
		if (materialID < m_StaticMesh->GetArrayCount()) {
			return m_Materials[materialID];
		}
		return nullptr;
	}


}
