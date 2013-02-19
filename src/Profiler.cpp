#include "Profiler.hpp"

lk::Profiler::Profiler () :
	clocks(std::vector<long>(10)),
	cpuClocks(std::vector<long>(10))
{
}

void lk::Profiler::setCheckPoint (size_t n)
{
	long k = cv::getTickCount();
	long l = cv::getCPUTickCount();
	if (n >= clocks.size())
	{
		clocks.resize(n);	
	}
	clocks[n] = k;
	if (n >= cpuClocks.size())
	{
		cpuClocks.resize(n);	
	}
	cpuClocks[n] = l;
}

const long lk::Profiler::getElapsedTicks (size_t n) const
{
	long k = cv::getTickCount();
	if (n < clocks.size())
	{
		return k - clocks[n];
	}
	else
	{
		std::cout << "Tried to access unset clock!" << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

const double lk::Profiler::getElapsedSeconds (size_t n) const
{
	long k = cv::getTickCount();
	if (n < clocks.size())
	{
		return (double(k) - clocks[n]) / cv::getTickFrequency();
	}
	else
	{
		std::cout << "Tried to access unset clock!" << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

std::ostream& operator<<(std::ostream& ostr, const lk::Profiler& pr)
{
	return ostr << "Profiler: []";
}
