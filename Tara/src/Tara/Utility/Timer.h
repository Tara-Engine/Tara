#pragma once
#include "tarapch.h"
#include <chrono>

namespace Tara {
	/// <summary>
	/// A high-accuracy timer class, used for timing things and profiling. Timing is done in microseconds
	/// </summary>
	/// <typeparam name="Fn">a callable, takes 2 paramaters: (const char*, float)</typeparam>
	template<typename Fn>
	class Timer {
	public:
		/// <summary>
		/// Create a new timer. The timer starts automatically.
		/// </summary>
		/// <param name="name">The timer's name, for logging. const char* is used to avoid heap-allocated strings. Ideally, this string is a literal.</param>
		/// <param name="func">A callable, with 2 paramaters: (const char*, float). Called when the timer stops.</param>
		Timer(const char* name, Fn&& func)
			:m_Name(name), m_Stopped(false), m_Func(func)
		{
			m_StartTime = std::chrono::high_resolution_clock::now();
		}

		
		~Timer() {
			if (!m_Stopped) {
				Stop();
			}
		}

		/// <summary>
		/// Stop the timer, returning the microseconds since it started.
		/// </summary>
		void Stop() {
			//must be first thing for accurate timing
			auto endTime = std::chrono::high_resolution_clock::now();
			//calculate duration
			long long startMS = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
			long long endMS = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();
			m_Stopped = true;
			float duration = (endMS - startMS) * 0.001f;
			//send data where it is needed.
			m_Func(m_Name, duration);
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
		bool m_Stopped;
		Fn m_Func;
	};

}