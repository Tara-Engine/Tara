#pragma once
#include "Tara/Core/Layer.h"
#include "Tara/Core/Entity.h"
#include "Tara/Asset/StaticMesh.h"

namespace Tara {

	REFTYPE(StaticMeshEntity);
	NOREFTYPE(StaticMeshEntity);

	class StaticMeshEntity : public Entity {
	public:
		StaticMeshEntity(
			EntityNoRef parent,
			LayerNoRef owningLayer,
			Transform transform = TRANSFORM_DEFAULT,
			StaticMeshRef mesh = nullptr,
			ShaderRef shader = nullptr,
			const std::string& name = "StaticMeshEntity"
		);

		virtual ~StaticMeshEntity() = default;

		virtual void OnDraw(float deltaTime) override;

		inline void SetStaticMesh(StaticMeshRef mesh) { m_StaticMesh = mesh; };

		inline const StaticMeshRef& GetStaticMesh() const { return m_StaticMesh; }
		
		void SetShader(int shaderID, ShaderRef shader);

		const ShaderRef& GetShader(int shaderID) const;

	private:
		StaticMeshRef m_StaticMesh;
		std::vector<ShaderRef> m_Shaders;

	};

}