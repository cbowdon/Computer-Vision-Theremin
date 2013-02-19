#ifndef HANDTRACKER_H
#define HANDTRACKER_H

#include <string>
#include <ostream>
#include <sstream>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include "PointAccountant.hpp"

namespace lk
{
	class HandTracker
	{
		public:

			HandTracker (const cv::Size& frameSize);
			void update (cv::Mat& frame);
			const cv::Point& getPos () const;
			const std::string report () const;

		private:

			void sharpen (const cv::Mat& in, cv::Mat& out) const;

			const cv::TermCriteria termCrit;
			const cv::Size winSize;
			const cv::Size frameSize;

			cv::Mat prevImageLK;

			PointAccountant pa;
			LKData data;
			cv::Point pos;

			std::vector<LKStats> stats;
	};
}
extern std::ostream& operator<<(std::ostream& ostr, const lk::HandTracker& ht);
#endif
