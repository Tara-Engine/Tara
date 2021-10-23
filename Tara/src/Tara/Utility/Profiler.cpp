#include "tarapch.h"
#include "Profiler.h"

namespace Tara {
	namespace Profiler {

		static std::unordered_map<std::string, std::vector<float>> ProfileData;

		void Log(const char* file, const char* name, float time)
		{
			std::string fullName = std::string(file);
			fullName = fullName.substr(fullName.find_last_of("/\\") + 1);
			fullName += ":";
			fullName += name;
			auto i = ProfileData.find(fullName);
			if (i == ProfileData.end()) {
				ProfileData[fullName] = std::vector<float>({time});
			}
			else {
				ProfileData[fullName].push_back(time);
			}
		}

		void Dump(int32_t logLevel)
		{
			std::stringstream ss;
			ss << "Profiling:" << std::endl << std::string(50,'=') << std::endl;
			ss << "Name\tCounts\tAverage\tMin\tMax\tStandard Deviation" << std::endl;
			for (auto [k, v] : ProfileData) {
				//begin by basic name and counts
				ss << k << "\t" << v.size() << "\t";
				//average
				float average = 0;
				for (auto time : v) { average += time; }
				average /= v.size();
				ss << average << "ms\t";
				//min and max
				auto minmax = std::minmax_element(v.begin(), v.end());
				ss << *(minmax.first) << "ms\t" << *(minmax.second) << "ms\t";
				//standard deviation

				float stdev = 0;
				for (auto time : v) { stdev += std::pow(std::abs(time - average), 2.0f); }
				stdev = std::sqrt(stdev / v.size());
				ss << stdev << std::endl;
			}
			ss << std::string(50, '=');
			VLOG_S(logLevel) << ss.str();
		}
	}
}
