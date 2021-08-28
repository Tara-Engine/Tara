#pragma once
#include "tarapch.h"

namespace Tara {
	enum class EventClass {
		None = 0,
		WindowClose, WindowResize, 
		KeyPress, KeyRelease, KeyType,
		MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll
		//TODO: collision, hit, etc. Application Events
	};

	enum EventCategory {
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

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	//ALL events (collisiton, mousemoved, keypressed, hit, etc.)
	class Event {
		//so that the EventFilter can view Event's private and protected members
		friend class EventFilter; 
	public:
		virtual EventClass GetEventClass() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }
		inline bool Handled() const { return m_Handled; }
		inline bool isInCategory(EventCategory category) { return GetCategoryFlags() & category; }
	protected:
		bool m_Handled = false;
	};

	//native window events
	class NativeEvent : public Event {
		EVENT_CLASS_CATEGORY(EventCategoryNative)
	};

	//game events
	class ApplicationEvent : public Event {
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	//used for filtering events to various specific event functions
	class EventFilter {
	public:
		EventFilter(Event& e)
			: m_Event(e) {}
		template<typename T, typename F> bool Dispatch(const F& func) {
			if (m_Event.GetEventClass() == T::GetStaticClass()) {
				m_Event.m_Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};
	
	//used for logging events
	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}

	/*----------------------------------------------------------------------------------\
	|							Individual Event Classes								|
	\----------------------------------------------------------------------------------*/

	class WindowCloseEvent : public NativeEvent {
	public:
		WindowCloseEvent(){}
		EVENT_CLASS_CLASS(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryNative)
	};

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

	//base class for key events
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

	//base class for mouse button events
	class MouseButtonEvent : public NativeEvent {
	public:
		MouseButtonEvent(int32_t button)
			: m_Button(button) {}
		inline int32_t getButton() const { return m_Button; }
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryMouseButton | EventCategoryInput)
	protected:
		int32_t m_Button;
	};

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