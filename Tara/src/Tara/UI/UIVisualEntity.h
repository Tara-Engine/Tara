#pragma once
#include "Tara/UI/UIBaseEntity.h"
#include "Tara/Asset/Patch.h"

namespace Tara {
	REFTYPE(UIVisualEntity);
	NOREFTYPE(UIVisualEntity);

	/// <summary>
	/// A UIEntity with some visuals. Namely a Patch.
	/// </summary>
	class UIVisualEntity : public UIBaseEntity {
	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="parent">the parent entity</param>
		/// <param name="owningLayer">the owning layer</param>
		/// <param name="patch">the visual patch</param>
		/// <param name="name">the name</param>
		UIVisualEntity(EntityNoRef parent, LayerNoRef owningLayer, PatchRef patch, const std::string& name = "UIVisualEntity");

		virtual ~UIVisualEntity() = default;

		/// <summary>
		/// Get the visual patch for this entity
		/// </summary>
		/// <returns></returns>
		inline const PatchRef& GetPatch() const { return m_Patch; }

		/// <summary>
		/// Set the visual patch for this entity
		/// </summary>
		/// <param name="patch"></param>
		inline void SetPatch(const PatchRef& patch) { m_Patch = patch, m_DesiredSizeDirty = true; }

		/// <summary>
		/// Set the visual tint
		/// </summary>
		/// <param name="color"></param>
		inline void SetTint(glm::vec4 color) { m_Tint = color; }
		
		/// <summary>
		/// Get the visual tint
		/// </summary>
		/// <returns></returns>
		inline const glm::vec4& GetTint() const { return m_Tint; }

		/// <summary>
		/// Set the border to be the same as the current patch pixel border
		/// </summary>
		void SetBorderFromPatch();
	public:

		/// <summary>
		/// Overridable calculate desired size. Must set it into m_DesieredSize;
		/// </summary>
		//virtual void CalculateDesiredSize();
		
		//void OnUpdate(float deltaTime);

		virtual void OnDraw(float deltaTime);

		inline virtual void OnBeginPlay() override {}

	private:
		PatchRef m_Patch;
		glm::vec4 m_Tint;
	};
}