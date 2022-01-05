#pragma once
#include "Tara/Core/Layer.h"
#include "Tara/Core/Entity.h"
#include "Tara/Asset/StaticMesh.h"
#include "Tara/Renderer/MaterialBase.h"

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
			MaterialBaseRef material = nullptr,
			const std::string& name = "StaticMeshEntity"
		);

		virtual ~StaticMeshEntity() = default;

		virtual void OnDraw(float deltaTime) override;

		inline void SetStaticMesh(StaticMeshRef mesh) { m_StaticMesh = mesh; };

		inline const StaticMeshRef& GetStaticMesh() const { return m_StaticMesh; }
		
		void SetMaterial(int materialID, MaterialBaseRef material);

		const MaterialBaseRef& GetMaterial(int materialID) const;

	private:
		StaticMeshRef m_StaticMesh;
		std::vector<MaterialBaseRef> m_Materials;

	};

}