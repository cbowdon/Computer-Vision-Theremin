#include "PointAccountant.hpp"

lk::PointAccountant::PointAccountant (const cv::Size& frameSize, const size_t idealPoints, const bool applyMinMove) :
	applyMinMove(applyMinMove),
	bounds(frameSize),
	distThreshold(5),
	errorThreshold(20),
	idealPoints(idealPoints),
	prevCenter(cv::Point2f(bounds.width / 2, bounds.height / 2))
{
}

void lk::PointAccountant::spawnPoints (std::vector<cv::Point2f>& points)
{
	if (points.empty())
	{
		spawnPointStar(prevCenter, points, idealPoints);
	}
	else if (points.size() < idealPoints)
	{
		// TODO this loop should be over ordering by most moved
		size_t k = 0;
		while (points.size() < idealPoints)
		{
			spawnPointStar(points[k], points, 4);
			k++;
		}	
	}
}

void lk::PointAccountant::weedPoints (lk::LKData& data, const bool respawn)
{
	// refuse to move if outside bounds
	cv::Point2f nextCenter = getCenter(data.nextPts);
	if (withinBounds(nextCenter))
	{
		prevCenter = nextCenter;
	}

	// collapse the stack
	size_t i, k;
	for(i = k = 0; i < data.nextPts.size(); i++)
	{

		// mark status = 0 for non-movers
		if (applyMinMove)
		{
			cv::Point2f dist = data.nextPts[i] - data.prevPts[i];
			if (std::abs(dist.x) < distThreshold && std::abs(dist.y) < distThreshold)
			{
				data.status[i] = 0;
			}
		}

		//mark status = 0 for out of bounds
		if (data.nextPts[i].x >= bounds.width || data.nextPts[i].y >= bounds.height)
		{
			data.status[i] = 0;
		}

		// mark status = 0 for high errors
		if (data.error[i] >= errorThreshold)
		{
			data.status[i] = 0;
		}

		if( !data.status[i] )
		{
			continue;
		}

		data.nextPts[k++] = data.nextPts[i];
	}
	data.nextPts.resize(k);

	if (respawn && k < idealPoints)
	{
		spawnPoints(data.nextPts);
	}
}

const cv::Point2f lk::PointAccountant::getCenter (const std::vector<cv::Point2f>& points) const
{
	cv::Point2f center;
	for (auto& pt : points)
	{
		center += pt;
	}

	center.x /= points.size();
	center.y /= points.size();

	return center;
}

const cv::Point2f lk::PointAccountant::getRange (const std::vector<cv::Point2f>& points) const
{
	float maxX = 0, maxY = 0, minX = bounds.width, minY = bounds.height;
	std::vector<cv::Point2f>::const_iterator it = points.begin();
	for (; it != points.end(); ++it)
	{
		if (it->x > maxX)
		{
			maxX = it->x;
		}
		if (it->y > maxY)
		{
			maxY = it->y;
		}
		if (it->x < minX)
		{
			minX = it->x;
		}
		if (it->y < minY)
		{
			minY = it->y;
		}
	}
	return cv::Point2f(maxX - minX, maxY - minY);
}

const cv::Size& lk::PointAccountant::getBounds () const
{
	return bounds;
}

const cv::Point2f& lk::PointAccountant::getPrevCenter () const
{
	return prevCenter;
}

const lk::LKStats lk::PointAccountant::getDataStats (const lk::LKData& data) const
{
	lk::LKStats stats;
	stats.center = getCenter(data.nextPts);
	stats.range = getRange(data.nextPts);
//	stats.stDev = getStDev(data.nextPts);

	return stats;
}

void lk::PointAccountant::spawnPointStar (const cv::Point2f& centerPoint, std::vector<cv::Point2f>& points, const size_t maxPoints)
{
	double starRadius = bounds.height / 50.0;
	for (size_t k = 0; k * 4 < maxPoints; k++)
	{
		// N
		points.push_back(centerPoint + cv::Point2f(0, starRadius * k));
		// S
		points.push_back(centerPoint + cv::Point2f(0, -starRadius * k));
		// E
		points.push_back(centerPoint + cv::Point2f(k * starRadius, 0));
		// W
		points.push_back(centerPoint + cv::Point2f(-starRadius * k, 0));
	}
}

bool lk::PointAccountant::withinBounds (const cv::Point2f& pt)
{
	return pt.x >= 0 && 
			pt.y >= 0 &&
			pt.x < bounds.width &&
			pt.y < bounds.height;
}
