#pragma once
#include "Tara/UI/UIBaseEntity.h"

namespace Tara {
	REFTYPE(UISpacerEntity);
	NOREFTYPE(UISpacerEntity);

	/// <summary>
	/// UISpacerEntity is an UIEntity that can have its minimum size set, 
	/// useful for enforcing size constraints, adding spaces to lists
	/// or general layout purposes.
	/// </summary>
	class UISpacerEntity : public UIBaseEntity {
	public:
		UISpacerEntity(EntityNoRef parent, LayerNoRef owningLayer, const std::string& name = "UISpacerEntity");

		virtual ~UISpacerEntity() = default;

		/// <summary>
		/// Set the minimum size of the UIEntity
		/// </summary>
		/// <param name="x">size in X</param>
		/// <param name="y">size in Y</param>
		inline void SetSize(float x, float y) { SetSize(glm::vec2{ x,y }); }

		/// <summary>
		/// Set the minimum size of the UIEntity
		/// </summary>
		/// <param name="size">the size</param>
		inline void SetSize(glm::vec2 size) { m_Size = size; }

		/// <summary>
		/// Get the minimum size of the UIEntity
		/// </summary>
		/// <returns></returns>
		inline const glm::vec2& GetSize() const { return m_Size; }


	public:
		//overriden stuff
		virtual void CalculateDesiredSize() override;

		inline virtual void OnBeginPlay() override {}
	private:
		glm::vec2 m_Size;
	};
}