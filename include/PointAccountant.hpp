#ifndef POINTACCOUNTANT_H
#define POINTACCOUNTANT_H

#include <string>
#include <ostream>
#include <iostream>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include "LKData.hpp"
#include "LKStats.hpp"

namespace lk
{
	class PointAccountant
	{
		public:
			PointAccountant (const cv::Size& frameSize, const size_t idealPoints=10, bool applyMinMove=false);
			void spawnPoints (std::vector<cv::Point2f>& points);
			void weedPoints (lk::LKData& data, const bool respawn=true);

			const cv::Point2f getCenter (const std::vector<cv::Point2f>& points) const;
			const cv::Point2f& getPrevCenter () const;

			const cv::Size& getBounds () const;
			const cv::Point2f getRange (const std::vector<cv::Point2f>& points) const;

			const LKStats getDataStats (const LKData& data) const;

		private:

			void spawnPointStar(const cv::Point2f& point, std::vector<cv::Point2f>& points, const size_t maxPoints);
			bool withinBounds (const cv::Point2f& pt);

			const bool applyMinMove;
			const cv::Size bounds;
			const float distThreshold;
			const float errorThreshold;
			const size_t idealPoints;
			cv::Point2f prevCenter;
	};
}
extern std::ostream& operator<<(std::ostream& ostr, const lk::PointAccountant& pa);
#endif
