#ifndef PROFILER_H
#define PROFILER_H

#include <string>
#include <ostream>
#include <iostream>
#include <opencv2/core/core.hpp>

namespace lk
{
	class Profiler
	{
		public:
			Profiler ();
			void setCheckPoint (size_t n = 0);
			const long getElapsedTicks (size_t n = 0) const;
			const double getElapsedSeconds (size_t n = 0) const;
		private:
			std::vector<long> clocks;
			std::vector<long> cpuClocks;
	};
}

extern std::ostream& operator<<(std::ostream& ostr, const lk::Profiler& pr);

#endif
