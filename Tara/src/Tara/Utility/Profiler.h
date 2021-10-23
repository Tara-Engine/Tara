#pragma once
#include "tarapch.h"
#include "Tara/Utility/Timer.h"

#ifdef TARA_PROFILING_ENABLED
#define SCOPE_PROFILE(name) Tara::Timer timer##__LINE__(name, [&](const char* n, float t){Tara::Profiler::Log(__FILE__, n, t);})
#else
#define SCOPE_PROFILE(name)
#endif

namespace Tara {
	/// <summary>
	/// A namespace to hold profiling functions.
	/// These functions are used by the internal profiling system to calculate metrics for code profiling
	/// </summary>
	namespace Profiler {
		/// <summary>
		/// Log a profiling event
		/// </summary>
		/// <param name="file">the associated file</param>
		/// <param name="name">the profiler name</param>
		/// <param name="time">the time</param>
		void Log(const char* file, const char* name, float time);

		/// <summary>
		/// Dump the stored profiling data to log (which also clears it)
		/// </summary>
		/// <param name="logLevel">the log level to dump to. range of 0 - 15. Defaults to 0 (which prints to console). Higher numbers = less importance</param>
		void Dump(int32_t logLevel = 0);
	
		//static std::unordered_map<std::string, std::vector<float>> s_ProfileData;
	}

}