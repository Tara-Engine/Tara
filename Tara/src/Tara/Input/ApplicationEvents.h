#pragma once
#include "Tara/Input/Event.h"
#include "Tara/Input/Manifold.h"
#include "Tara/Core/Entity.h"
#include "Tara/Core/Component.h"

namespace Tara {
	
	//REFTYPE(Entity);
	//NOREFTYPE(Entity);

	//REFTYPE(Component);
	//NOREFTYPE(Component);

	REFTYPE(UIBaseEntity);
	NOREFTYPE(UIBaseEntity);
	
	/// <summary>
	/// Overlap Event
	/// created when two bodies overlap. 
	/// The same event is not delivered to both bodies, 
	/// indeed, they each get their own copy.
	/// Each gets one that has the A reference being itself
	/// </summary>
	class OverlapEvent : public ApplicationEvent {
	public:
		OverlapEvent(const Manifold& manifold)
			 :m_Manifold(manifold)
		{}
		/// <summary>
		/// Get the self reference from the manifold.
		/// </summary>
		/// <returns></returns>
		virtual EntityRef GetSelf() const { return m_Manifold.A; }
		/// <summary>
		/// Get the other reference from the manifold
		/// </summary>
		/// <returns></returns>
		virtual EntityRef GetOther() const { return m_Manifold.B; }
		/// <summary>
		/// Get the ammount the objects are overlapping along normal
		/// </summary>
		/// <returns></returns>
		virtual float GetPenetration() const { return m_Manifold.Penetration; }
		/// <summary>
		/// Get the shortest direction to not overlapping. (2D X-Y plane)
		/// </summary>
		/// <returns></returns>
		virtual Vector GetNormal() const { return m_Manifold.Normal; }

		virtual std::string ToString() const override;

		EVENT_CLASS_CLASS(Overlap)
		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryOverlap)
	protected:
		const Manifold& m_Manifold;
	};

	/// <summary>
	/// Base class for events relating to children
	/// </summary>
	/// <returns></returns>
	class ChildEvent : public ApplicationEvent {
	public:
		ChildEvent(EntityNoRef parent, EntityNoRef child)
			:m_Parent(parent), m_Child(child)
		{}
		/// <summary>
		/// Get the parent in the event
		/// </summary>
		/// <returns></returns>
		virtual inline EntityRef GetParent() const { return m_Parent.lock(); }
		/// <summary>
		/// Get the child in the event
		/// </summary>
		/// <returns></returns>
		virtual inline EntityRef GetChild() const { return m_Child.lock(); }
		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryHirarchy)

	protected:
		inline std::string GetParentName() const { return ((GetParent()) ? GetParent()->GetName() : "[NULL]"); }
		inline std::string GetChildName() const {return ((GetChild()) ? GetChild()->GetName() : "[NULL]");}
	private:
		EntityNoRef m_Parent;
		EntityNoRef m_Child;
	};

	/// <summary>
	/// Event for when a child is added to a parent.
	/// </summary>
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

	/// <summary>
	/// Event for when a child is removed from a parent
	/// </summary>
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

	/// <summary>
	/// Event for when an entity has its parents swapped.
	/// </summary>
	class ParentSwapedEvent : public ApplicationEvent {
	public:
		ParentSwapedEvent(EntityNoRef oldParent, EntityNoRef newParent)
			:m_OldParent(oldParent), m_NewParent(newParent)
		{}
		/// <summary>
		/// Get the old parent
		/// </summary>
		/// <returns></returns>
		virtual inline EntityRef GetOldParent() const { return m_OldParent.lock(); }
		/// <summary>
		/// Get the new parent
		/// </summary>
		/// <returns></returns>
		virtual inline EntityRef GetNewParent() const { return m_NewParent.lock(); }
		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "Parents Swapped Event : Old Parent:\"" << GetOldParentName() << "\", New Parent:\"" << GetNewParentName() << "\"";
			return ss.str();
		}
		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryHirarchy)
		EVENT_CLASS_CLASS(ParentChanged)
	protected:
		virtual inline std::string GetOldParentName() const { return ((GetOldParent()) ? GetOldParent()->GetName() : "[NULL]"); }
		virtual inline std::string GetNewParentName() const { return ((GetNewParent()) ? GetNewParent()->GetName() : "[NULL]"); }
	private:
		EntityNoRef m_OldParent;
		EntityNoRef m_NewParent;
	};

	/// <summary>
	/// Base class for component-related events
	/// </summary>
	/// <returns></returns>
	class ComponentEvent : public ApplicationEvent {
	public:
		ComponentEvent(EntityNoRef entity, ComponentNoRef component)
			:m_Entity(entity), m_Component(component)
		{}
		/// <summary>
		/// Get the entity in the event
		/// </summary>
		/// <returns></returns>
		virtual inline EntityRef GetEntity() const { return m_Entity.lock(); }
		/// <summary>
		/// Get the component in the event
		/// </summary>
		/// <returns></returns>
		virtual inline ComponentRef GetComponent() const { return m_Component.lock(); }
		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryHirarchy)

	protected:
		inline std::string GetEntityName() const { return ((GetEntity()) ? GetEntity()->GetName() : "[NULL]"); }
		inline std::string GetComponentName() const { return ((GetComponent()) ? GetComponent()->GetName() : "[NULL]"); }
	private:
		EntityNoRef m_Entity;
		ComponentNoRef m_Component;
	};

	/// <summary>
	/// Event for when a component is added to an entity
	/// </summary>
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

	/// <summary>
	/// Event for when a component is removed from an entity
	/// </summary>
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

	/// <summary>
	/// Event for when an entity is clicked on. Must have a ClickableComponent attached to receive.
	/// </summary>
	class ClickEvent : public ApplicationEvent {
	public:
		ClickEvent(float mouseX, float mouseY, int32_t button, bool release = false, bool mouseMoveOff = false)
			: m_MouseX(mouseX), m_MouseY(mouseY), m_Button(button), m_Release(release), m_MouseMoveOff(mouseMoveOff)
		{}
		/// <summary>
		/// Get the mouse  position
		/// </summary>
		/// <returns></returns>
		inline glm::vec2 GetMousePos() const { return { m_MouseX, m_MouseY }; }
		/// <summary>
		/// Get the mouse X position
		/// </summary>
		/// <returns></returns>
		inline float GetMouseX() const { return m_MouseX; }
		/// <summary>
		/// Get the mouse Y position
		/// </summary>
		/// <returns></returns>
		inline float GetMouseY() const { return m_MouseY; }
		/// <summary>
		/// Get the highest-priority button that is down (TARA_BUTTON_1 has the highest, TARA_BUTTON_8 has the lowest)
		/// </summary>
		/// <returns></returns>
		inline int32_t GetMouseButton() const { return m_Button; }

		/// <summary>
		/// Get if the button was pressed or released. When released, this is true. It is false if the mouse moves off the element.
		/// </summary>
		/// <returns></returns>
		inline bool GetRelease() const { return m_Release; }

		/// <summary>
		/// Get if the element is Unclicked because the mouse moved off of it
		/// GetRelease will be false
		/// </summary>
		/// <returns></returns>
		inline bool GetMouseMoveOff() const { return m_MouseMoveOff; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "Click Event: Mouse Pos: [X=" << m_MouseX << " Y=" << m_MouseY << "]. Button: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryUI)
		EVENT_CLASS_CLASS(ClickEvent)
	private:
		float m_MouseX;
		float m_MouseY;
		int32_t m_Button;
		bool m_Release;
		bool m_MouseMoveOff;
	};

	/// <summary>
	/// mouse HoverEvent. Must have a ClickableComponent attached to receive. You will receive one with isHovering set to false when the mouse stops hovering.
	/// </summary>
	class HoverEvent : public ApplicationEvent {
	public:
		HoverEvent(float mouseX, float mouseY, bool isHovering = true)
			:m_MouseX(mouseX), m_MouseY(mouseY), m_IsHovering(isHovering)
		{}

		/// <summary>
		/// Get the mouse  position
		/// </summary>
		/// <returns></returns>
		inline glm::vec2 GetMousePos() const { return { m_MouseX, m_MouseY }; }
		/// <summary>
		/// Get the mouse X position
		/// </summary>
		/// <returns></returns>
		inline float GetMouseX() const { return m_MouseX; }
		/// <summary>
		/// Get the mouse Y position
		/// </summary>
		/// <returns></returns>
		inline float GetMouseY() const { return m_MouseY; }

		/// <summary>
		/// Get if this is an actual hover event or an end hover event. You will recive one with this set to false when the mouse stops hovering
		/// </summary>
		/// <returns></returns>
		inline bool GetIsHovering() const { return m_IsHovering; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "Hover Event";
			if (!m_IsHovering) {
				ss << " Ended";
			}
			ss << ": Mouse Pos: [X=" << m_MouseX << " Y=" << m_MouseY << "].";
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryUI)
		EVENT_CLASS_CLASS(HoverEvent)
	private:
		float m_MouseX;
		float m_MouseY;
		bool m_IsHovering;
	};

	/// <summary>
	/// An event for when the mouse is clicked, then moved while held down
	/// </summary>
	class DragEvent : public ApplicationEvent {
	public:
		enum class DragType : uint8_t {
			BEGIN, CONTINUE, END
		};
	public:
		DragEvent(float mouseX, float mouseY, DragType type)
			:m_MouseX(mouseX), m_MouseY(mouseY), m_Type(type)
		{}
		/// <summary>
		/// Get the mouse  position
		/// </summary>
		/// <returns></returns>
		inline glm::vec2 GetMousePos() const { return { m_MouseX, m_MouseY }; }
		/// <summary>
		/// Get the mouse X position
		/// </summary>
		/// <returns></returns>
		inline float GetMouseX() const { return m_MouseX; }
		/// <summary>
		/// Get the mouse Y position
		/// </summary>
		/// <returns></returns>
		inline float GetMouseY() const { return m_MouseY; }

		/// <summary>
		/// Get the drag type (start, continue, end)
		/// </summary>
		/// <returns></returns>
		inline DragType GetDragType() const { return m_Type; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "Drag Event: Mouse Pos: [X=" << m_MouseX << " Y=" << m_MouseY << "]. Type: ";
			switch (m_Type) {
			case DragType::BEGIN:{ ss << "BEGIN."; break; }
			case DragType::CONTINUE:{ ss << "CONTINUE."; break; }
			case DragType::END:{ ss << "END."; break; }
			}
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryUI)
		EVENT_CLASS_CLASS(DragEvent)
	private:
		float m_MouseX;
		float m_MouseY;
		DragType m_Type;
	};

	/// <summary>
	/// UIToggleEvent is delivered by UI elements that have a binary on/off state, like checkboxes and buttons.
	/// </summary>
	class UIToggleEvent : public ApplicationEvent {
	public:
		UIToggleEvent(UIBaseEntityNoRef element, bool toggle = true)
			:m_Element(element), m_Toggle(toggle)
		{}

		/// <summary>
		/// Get the UIEntity that had the event
		/// </summary>
		/// <returns></returns>
		inline UIBaseEntityNoRef GetElement() const { return m_Element; }

		/// <summary>
		/// Get the toggle state. For some UI like buttons, this will always be true.
		/// For others, like checkboxes, it may be true or false.
		/// </summary>
		/// <returns></returns>
		inline bool GetToggle() const { return m_Toggle; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "UI Toggle Event: State: " << ((m_Toggle)?"On":"Off");
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryUI)
		EVENT_CLASS_CLASS(UIToggleEvent)
	private:
		UIBaseEntityNoRef m_Element;
		bool m_Toggle;
	};

	/// <summary>
	/// UIStateChangeEvent is delivered by UI elements that have complex input, like a TextEntryField or a DropdownBox, when its state is changed by the user.
	/// The specific state that is changed is not held by the event, as it is complex.
	/// </summary>
	class UIStateChangeEvent : public ApplicationEvent {
	public:
		UIStateChangeEvent(UIBaseEntityNoRef element)
			:m_Element(element)
		{}

		/// <summary>
		/// Get the UIEntity that had the event
		/// </summary>
		/// <returns></returns>
		inline UIBaseEntityNoRef GetElement() const { return m_Element; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "UI State Change Event.";
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryUI)
		EVENT_CLASS_CLASS(UIStateChangeEvent)
	private:
		UIBaseEntityNoRef m_Element;
	};
}