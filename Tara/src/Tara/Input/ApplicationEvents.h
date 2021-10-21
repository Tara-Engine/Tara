#pragma once
#include "Tara/Input/Event.h"
#include "Tara/Input/Manifold.h"
#include "Tara/Core/Entity.h"
#include "Tara/Core/Component.h"

namespace Tara {
	
	//REFTYPE(Entity);
	//NOREFTYPE(Entity);

	REFTYPE(Component);
	NOREFTYPE(Component);

	
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

	class ChildEvent : public ApplicationEvent {
	public:
		ChildEvent(EntityNoRef parent, EntityNoRef child)
			:m_Parent(parent), m_Child(child)
		{}
		virtual inline EntityRef GetParent() const { return m_Parent.lock(); }
		virtual inline EntityRef GetChild() const { return m_Child.lock(); }
		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryHirarchy)

	protected:
		inline std::string GetParentName() const { return ((GetParent()) ? GetParent()->GetName() : "[DESTROYED]"); }
		inline std::string GetChildName() const {return ((GetChild()) ? GetChild()->GetName() : "[DESTROYED]");}
	private:
		EntityNoRef m_Parent;
		EntityNoRef m_Child;
	};

	class ChildAddedEvent : public ChildEvent {
	public:
		ChildAddedEvent(EntityNoRef parent, EntityNoRef child)
			: ChildEvent(parent, child)
		{}
		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "Child Added to parent: Parent:\"" << GetParentName() << "\", Child:\"" << GetChildName() << "\"";
			return ss.str();
		}
		EVENT_CLASS_CLASS(ChildAdded)
	};

	class ChildRemovedEvent : public ChildEvent {
	public:
		ChildRemovedEvent(EntityNoRef parent, EntityNoRef child)
			: ChildEvent(parent, child)
		{}
		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "Child Removed from parent: Parent:\"" << GetParentName() << "\", Child:\"" << GetChildName() << "\"";
			return ss.str();
		}
		EVENT_CLASS_CLASS(ChildRemoved)
	};

	class ParentSwapedEvent : public ApplicationEvent {
	public:
		ParentSwapedEvent(EntityNoRef oldParent, EntityNoRef newParent)
			:m_OldParent(oldParent), m_NewParent(newParent)
		{}
		virtual inline EntityRef GetOldParent() const { return m_OldParent.lock(); }
		virtual inline EntityRef GetNewParent() const { return m_NewParent.lock(); }
		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "Parents Swapped Event : Old Parent:\"" << GetOldParentName() << "\", New Parent:\"" << GetNewParentName() << "\"";
			return ss.str();
		}
		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryHirarchy)
		EVENT_CLASS_CLASS(ParentChanged)
	protected:
		virtual inline std::string GetOldParentName() const { return ((GetOldParent()) ? GetOldParent()->GetName() : "[DESTROYED]"); }
		virtual inline std::string GetNewParentName() const { return ((GetNewParent()) ? GetNewParent()->GetName() : "[DESTROYED]"); }
	private:
		EntityNoRef m_OldParent;
		EntityNoRef m_NewParent;
	};


	class ComponentEvent : public ApplicationEvent {
	public:
		ComponentEvent(EntityNoRef entity, ComponentNoRef component)
			:m_Entity(entity), m_Component(component)
		{}
		virtual inline EntityRef GetEntity() const { return m_Entity.lock(); }
		virtual inline ComponentRef GetComponent() const { return m_Component.lock(); }
		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryHirarchy)

	protected:
		inline std::string GetEntityName() const { return ((GetEntity()) ? GetEntity()->GetName() : "[DESTROYED]"); }
		inline std::string GetComponentName() const { return ((GetComponent()) ? GetComponent()->GetName() : "[DESTROYED]"); }
	private:
		EntityNoRef m_Entity;
		ComponentNoRef m_Component;
	};

	class ComponentAddedEvent : public ComponentEvent {
	public:
		ComponentAddedEvent(EntityNoRef parent, ComponentNoRef child)
			: ComponentEvent(parent, child)
		{}
		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "Component Added to Entity: Entity:\"" << GetEntityName() << "\", Component:\"" << GetComponentName() << "\"";
			return ss.str();
		}
		EVENT_CLASS_CLASS(ComponentAdded)
	};

	class ComponentRemovedEvent : public ComponentEvent {
	public:
		ComponentRemovedEvent(EntityNoRef parent, ComponentNoRef child)
			: ComponentEvent(parent, child)
		{}
		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "Component Removed from Entity: Entity:\"" << GetEntityName() << "\", Component:\"" << GetComponentName() << "\"";
			return ss.str();
		}
		EVENT_CLASS_CLASS(ComponentRemoved)
	};

}