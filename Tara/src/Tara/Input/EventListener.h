#pragma once
#include "Tara/Input/Event.h"
//#include "Tara/Input/ApplicationEvents.h"

namespace Tara {

	/// <summary>
	/// Class to handle Entities and Components that listen for Events
	/// </summary>
	class EventListener {
	public:

		/// <summary>
		/// Handle the event.
		/// </summary>
		/// <param name="e">the event to handle</param>
		inline virtual void OnEvent(Event& e) {}

		/// <summary>
		/// Receive the event. By default, just sends to OnEvent(). 
		/// Some subclasses may have more features though.
		/// </summary>
		/// <param name="e">The event</param>
		inline virtual void ReceiveEvent(Event& e) { OnEvent(e); }

		/// <summary>
		/// Enable/Disable Listening for native window events
		/// </summary>
		/// <param name="enable"></param>
		virtual void ListenForEvents(bool enable = true) = 0;

		/// <summary>
		/// Set the internal listening flag
		/// </summary>
		/// <param name="enabled"></param>
		inline void SetListeningForEvents(bool enabled) { m_ListeningForEvents = enabled; }

		/// <summary>
		/// Get the internal listening flag
		/// </summary>
		/// <returns></returns>
		inline bool GetListeningForEvents() const { return m_ListeningForEvents; }

	private:
		
		bool m_ListeningForEvents;
	};

	using EventListenerNoRef = std::weak_ptr<EventListener>;

	/// <summary>
	/// Equality operator for specifically EventListenerNoRef. 
	/// std::weak_ptr does not have operator== by default
	/// This operator is nesecary for using a std::list of these.
	/// Rather annoying, but whatever.
	/// </summary>
	/// <param name="lhs">the left hand side of the comparison</param>
	/// <param name="rhs">the right hand side of the comparison</param>
	/// <returns>true if equal, false otherwise</returns>
	inline bool operator==(const EventListenerNoRef& lhs, const EventListenerNoRef& rhs) {
		return lhs.lock() == rhs.lock();
	}
}