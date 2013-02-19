#ifndef LKSTATS_H
#define LKSTATS_H

#include <string>
#include <ostream>
#include "LKData.hpp"

namespace lk
{
	struct LKStats
	{
		cv::Point2f center;			
		cv::Point2f range;
		cv::Point2f stDev;
	};
}

inline std::ostream& operator<<(std::ostream& ostr, const lk::LKStats& stats)
{
	ostr << "LKStats [center = " << stats.center;
	ostr << ", range = " << stats.range;
	ostr << ", stDev = " << stats.stDev << "]";
	return ostr;
}

#endif
