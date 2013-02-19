#ifndef LKDATA_H
#define LKDATA_H

#include <vector>
#include <opencv2/core/core.hpp>

namespace lk
{
	struct LKData
	{
		std::vector<cv::Point2f> prevPts;
		std::vector<cv::Point2f> nextPts;
		std::vector<unsigned char> status;
		std::vector<float> error;
	};
}
#endif
