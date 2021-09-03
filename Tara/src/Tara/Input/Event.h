#pragma once
#include "tarapch.h"

namespace Tara {

	/// <summary>
	/// The Class of the event, for filtering
	/// </summary>
	enum class EventClass {
		None = 0,
		WindowClose, WindowResize, 
		KeyPress, KeyRelease, KeyType,
		MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll
		//TODO: collision, hit, etc. Application Events
	};

	/// <summary>
	/// the Category of the event. an event might have more than one, or'd together
	/// </summary>
	enum EventCategory : uint16_t {
		None = 0,
		EventCategoryNative		 = BIT(0),
		EventCategoryApplication = BIT(1), //FUTURE-PROOFING
		EventCategoryInput		 = BIT(2),
		EventCategoryKeyboard	 = BIT(3),
		EventCategoryMouse		 = BIT(4),
		EventCategoryMouseButton = BIT(5)
		//TODO: Categories of application events
	};

/*Helper macros for defining the repeditive stuff in events*/
#define EVENT_CLASS_CLASS(type) inline static EventClass GetStaticClass(){return EventClass::type;}\
								inline virtual EventClass GetEventClass() const override {return EventClass::type;}\
								inline virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual uint16_t GetCategoryFlags() const override { return category; }

	/// <summary>
	/// Event base class, all events are passed around as this.
	/// </summary>
	class Event {
		//so that the EventFilter can view Event's private and protected members
		friend class EventFilter; 
	public:
		/// <summary>
		/// Get the class of the event
		/// </summary>
		/// <returns>the class of teh event</returns>
		virtual EventClass GetEventClass() const = 0;
		/// <summary>
		/// Get a C-style strigified name of the event
		/// </summary>
		/// <returns>c-string name of the event</returns>
		virtual const char* GetName() const = 0;
		/// <summary>
		/// get the category flags of the event
		/// </summary>
		/// <returns></returns>
		virtual uint16_t GetCategoryFlags() const = 0;
		/// <summary>
		/// Get a C++-stype strigifiec name of the event
		/// </summary>
		/// <returns>std::string name of the event</returns>
		virtual std::string ToString() const { return GetName(); }
		/// <summary>
		/// Get if the event has been marked as handled or not
		/// </summary>
		/// <returns>true if handled, false otherwise</returns>
		inline bool Handled() const { return m_Handled; }
		/// <summary>
		/// check if the event is in a given category.
		/// </summary>
		/// <param name="category">the category to test for</param>
		/// <returns>bool if the event is in the category</returns>
		inline bool isInCategory(EventCategory category) { return GetCategoryFlags() & category; }
	protected:
		/// <summary>
		/// protected bool to store if the event has been handled.
		/// </summary>
		bool m_Handled = false;
	};

	
	/// <summary>
	/// Natvie Window Events
	/// </summary>
	class NativeEvent : public Event {
		EVENT_CLASS_CATEGORY(EventCategoryNative)
	};

	/// <summary>
	/// Engine- or Appliction-generated events
	/// Such as collision, trace hits, etc.
	/// </summary>
	class ApplicationEvent : public Event {
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	/// <summary>
	/// A filter to filer events based on class to different funcions.
	/// Usage:
	/// EventFilter filter(event);
	/// filter.Call&lt;EventSubclassName&gt;(TARA_BIND_FN(ClassName:FunctionName));
	/// filter.Call&lt;EventDifferentSubclassName&gt;(TARA_BIND_FN(ClassName:FunctionName));
	/// </summary>
	class EventFilter {
	public:
		/// <summary>
		/// Constructor for an Event Filter
		/// </summary>
		/// <param name="e">The event</param>
		EventFilter(Event& e)
			: m_Event(e) {}
		/// <summary>
		/// Attempt to call the supplied function with the event the filter was created with, 
		/// if it is the supplied subclass
		/// </summary>
		/// <typeparam name="T">EventSubclassType</typeparam>
		/// <typeparam name="F">function type, [bool](EventSubclassType e){...}</typeparam>
		/// <param name="func">the bound function</param>
		/// <returns>bool if the function could be called</returns>
		template<typename T, typename F> bool Call(const F& func) {
			if (m_Event.GetEventClass() == T::GetStaticClass()) {
				m_Event.m_Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		/// <summary>
		/// The stored event
		/// </summary>
		Event& m_Event;
	};
	
	/// <summary>
	/// Event logging through streams
	/// </summary>
	/// <param name="os">the stream</param>
	/// <param name="e">the event</param>
	/// <returns>the stream, again</returns>
	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}

	/*----------------------------------------------------------------------------------\
	|							Individual Event Classes								|
	\----------------------------------------------------------------------------------*/

	/// <summary>
	/// Window Closed Event
	/// Generated when the window is closed
	/// </summary>
	class WindowCloseEvent : public NativeEvent {
	public:
		WindowCloseEvent(){}
		EVENT_CLASS_CLASS(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryNative)
	};

	/// <summary>
	/// Window Resize Event
	/// Created when the window is resized
	/// </summary>
	class WindowResizeEvent : public NativeEvent {
	public:
		WindowResizeEvent(uint32_t width, uint32_t height)
			: m_Width(width), m_Height(height) {}

		inline uint32_t getWidth() const { return m_Width; }
		inline uint32_t getHeight() const { return m_Height; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << GetName() << " [X:" << m_Width << " Y:" << m_Height << "]";
			return ss.str();
		}

		EVENT_CLASS_CLASS(WindowResize)
			EVENT_CLASS_CATEGORY(EventCategoryNative)

	private:
		uint32_t m_Width, m_Height;
	};

	/// <summary>
	/// Key Event
	/// abstract base class for key events
	/// </summary>
	class KeyEvent : public NativeEvent {
	public:
		KeyEvent(int32_t key, int32_t mods)
			: m_Key(key), m_Mods(mods) {}
		inline int32_t getKey() const { return m_Key; }
		inline int32_t getMods() const { return m_Mods; }
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		int32_t m_Key, m_Mods;
	};

	/// <summary>
	/// Key Pressed Event
	/// Generated when a key is pressed or repeated
	/// </summary>
	class KeyPressEvent : public KeyEvent {
	public:
		KeyPressEvent(int32_t key, int32_t mods, int32_t repeatCount)
			: KeyEvent(key, mods), m_RepeatCount(repeatCount) {}
		inline int32_t getRepeatCount() const { return m_RepeatCount; }
		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << GetName() << " [Key:" << m_Key << " Mods:" << m_Mods << " Repeats:" << m_RepeatCount << "]";
			return ss.str();
		}
		EVENT_CLASS_CLASS(KeyPress)
	private:
		int32_t m_RepeatCount;
	};

	/// <summary>
	/// Key Released Event
	/// generated when a key is released
	/// </summary>
	class KeyReleaseEvent : public KeyEvent {
	public:
		KeyReleaseEvent(int32_t key, int32_t mods)
			: KeyEvent(key, mods){}
		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << GetName() << " [Key:" << m_Key << " Mods:" << m_Mods << "]";
			return ss.str();
		}
		EVENT_CLASS_CLASS(KeyRelease)
	};

	/// <summary>
	/// Key Typed Event
	/// generated when a typing key is pressed. The key is an ASCII keycode, and can be cast to char
	/// </summary>
	class KeyTypeEvent : public KeyEvent {
	public:
		KeyTypeEvent(int32_t key)
			: KeyEvent(key, 0) {}
		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << GetName() << " [Key:" << m_Key <<  " (" << (char)m_Key << ")]";
			return ss.str();
		}
		EVENT_CLASS_CLASS(KeyType)
	};
	
	/// <summary>
	/// Mouse Button Events
	/// base class for mouse button events
	/// </summary>
	class MouseButtonEvent : public NativeEvent {
	public:
		MouseButtonEvent(int32_t button)
			: m_Button(button) {}
		inline int32_t getButton() const { return m_Button; }
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryMouseButton | EventCategoryInput)
	protected:
		int32_t m_Button;
	};

	/// <summary>
	/// Mouse Button Press Event
	/// created when a mouse button is pressed
	/// </summary>
	class MouseButtonPressEvent : public MouseButtonEvent {
	public:
		MouseButtonPressEvent(int32_t button)
			: MouseButtonEvent(button) {}
		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << GetName() << " [Key:" << m_Button << "]";
			return ss.str();
		}
		EVENT_CLASS_CLASS(MouseButtonPress)
	};

	/// <summary>
	/// Mouse Button Release Event
	/// created when a mouse button is released
	/// </summary>
	class MouseButtonReleaseEvent : public MouseButtonEvent {
	public:
		MouseButtonReleaseEvent(int32_t button)
			: MouseButtonEvent(button) {}
		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << GetName() << " [Key:" << m_Button << "]";
			return ss.str();
		}
		EVENT_CLASS_CLASS(MouseButtonRelease)
	};

	/// <summary>
	/// Mouse Move Event
	/// created when the mouse is moved
	/// </summary>
	class MouseMoveEvent : public NativeEvent {
	public:
		MouseMoveEvent(float xPos, float yPos)
			: m_xPos(xPos), m_yPos(yPos) {}
		inline float GetXPos() const { return m_xPos; }
		inline float GetYPos() const { return m_yPos; }
		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << GetName() << " [X:" << m_xPos << " Y:" << m_yPos << "]";
			return ss.str();
		}
		EVENT_CLASS_CLASS(MouseMove)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		float m_xPos, m_yPos;
	};

	/// <summary>
	/// Mouse Scroll Event
	/// created when the mouse is scrolled.
	/// </summary>
	class MouseScrollEvent : public NativeEvent {
	public:
		MouseScrollEvent(float xOffset, float yOffset)
			: m_xOffset(xOffset), m_yOffset(yOffset) {}
		inline float GetXOffset() const { return m_xOffset; }
		inline float GetYOffset() const { return m_yOffset; }
		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << GetName() << " [X:" << m_xOffset << " Y:" << m_yOffset << "]";
			return ss.str();
		}
		EVENT_CLASS_CLASS(MouseScroll)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		float m_xOffset, m_yOffset;
	};

	//TODO: Add application events
}