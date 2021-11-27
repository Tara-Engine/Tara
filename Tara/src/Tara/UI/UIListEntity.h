#pragma once
#include "Tara/UI/UIBaseEntity.h"

namespace Tara {

	REFTYPE(UIListEntity);
	NOREFTYPE(UIListEntity);

	/// <summary>
	/// an invisible list. Each child gets its own slot to render in, listed vertically or horizontally, as set.
	/// </summary>
	class UIListEntity : public UIBaseEntity {
	public:
		/// <summary>
		/// direction of the list
		/// </summary>
		enum class Direction : bool {
			Vertical = 0,
			Horizontal = 1
		};

	public:
		UIListEntity(EntityNoRef parent, LayerNoRef owningLayer, const std::string& name = "UIListEntity");

		virtual ~UIListEntity() = default;

		/// <summary>
		/// Set the direction that the list orients its children
		/// </summary>
		/// <param name="dir"></param>
		inline void SetDirecton(Direction dir) { m_Direction = dir; }

		/// <summary>
		/// Get the direction that the list orients its children
		/// </summary>
		/// <returns></returns>
		inline Direction GetDirection() const { return m_Direction; }

	public:
		//overrides

		virtual void CalculateDesiredSize();

		void OnUpdate(float deltaTime);

		//for debug purposes
		void OnDraw(float deltaTime);
	private:
		Direction m_Direction;
	};

}