#pragma once
#include "Tara/Input/Event.h"


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
		OverlapEvent(EntityRef self, EntityRef other)
			 :m_Self(self), m_Other(other)
		{}
		virtual EntityRef GetSelf() const { return m_Self; }
		virtual EntityRef GetOther() const { return m_Other; }

		virtual OverlapEvent Invert() const { return OverlapEvent(m_Other, m_Self); }

		virtual std::string ToString() const override;

		EVENT_CLASS_CLASS(Overlap)
		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryOverlap)
	protected:
		EntityRef m_Self, m_Other;
	};
}