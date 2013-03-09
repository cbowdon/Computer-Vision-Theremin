#ifndef POINTACCOUNTANT_H
#define POINTACCOUNTANT_H

#include <ostream>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include "LKData.hpp"

namespace lk
{
	class PointAccountant
	{
		public:
			PointAccountant (const cv::Size& frameSize, const size_t idealPoints, bool applyMinMove);

			void spawn (const cv::Point2f& centerPoint, std::vector<cv::Point2f>& points, const size_t ptsToSpawn);
			void replenish (std::vector<cv::Point2f>& points);
			void weed (lk::LKData& data, const bool respawn=true);

			const cv::Point2f getCenter (const std::vector<cv::Point2f>& points) const;

			const cv::Size& getBounds () const;
			const cv::Point2f& getPrevCenter () const;

		private:

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
