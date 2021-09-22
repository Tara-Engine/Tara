#pragma once
#include "Tara/Input/Event.h"
#include "Tara/Input/Manifold.h"

namespace Tara {
	
	//can't include, due to circular includes
	class Entity;
	using EntityRef = std::shared_ptr<Entity>;
	
	/// <summary>
	/// Overlap Event
	/// created when two bodies overlap. 
	/// The same event is not delivered to both bodies, 
	/// indeed, they each get their own copy.
	/// Each gets one that has the A reference being itself
	/// </summary>
	class OverlapEvent : public ApplicationEvent {
	public:
		//TODO: Add manifold to event!
		OverlapEvent(const Manifold& manifold)
			 :m_Manifold(manifold)
		{}
		virtual EntityRef GetSelf() const { return m_Manifold.A; }
		virtual EntityRef GetOther() const { return m_Manifold.B; }
		virtual float GetPenetration() const { return m_Manifold.Penetration; }
		virtual Vector GetNormal() const { return m_Manifold.Normal; }

		virtual std::string ToString() const override;

		EVENT_CLASS_CLASS(Overlap)
		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryOverlap)
	protected:
		const Manifold& m_Manifold;
	};
}