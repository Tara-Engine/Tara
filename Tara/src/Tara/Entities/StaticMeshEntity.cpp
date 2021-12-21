#include "tarapch.h"
#include "StaticMeshEntity.h"
#include "Tara/Renderer/Renderer.h"

namespace Tara{
	StaticMeshEntity::StaticMeshEntity(EntityNoRef parent, LayerNoRef owningLayer, Transform transform, StaticMeshRef mesh, ShaderRef shader, const std::string& name)
		: Entity(parent, owningLayer, transform, name), m_StaticMesh(mesh), m_Shaders(mesh->GetArrayCount(), shader)
	{}

	void StaticMeshEntity::OnDraw(float deltaTime)
	{
		for (int i = 0; i < m_StaticMesh->GetArrayCount();i++) {
			Renderer::Draw(m_StaticMesh->GetVertexArrays()[i], m_Shaders[i], m_Transform);
		}
	}

	void StaticMeshEntity::SetShader(int shaderID, ShaderRef shader)
	{
		if (shaderID < m_StaticMesh->GetArrayCount()) {
			m_Shaders[shaderID] = shader;
		}
	}

	const ShaderRef& StaticMeshEntity::GetShader(int shaderID) const
	{
		if (shaderID < m_StaticMesh->GetArrayCount()) {
			return m_Shaders[shaderID];
		}
		return nullptr;
	}


}
