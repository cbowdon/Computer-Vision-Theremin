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

void lk::PointAccountant::spawn (const cv::Point2f& centerPoint, std::vector<cv::Point2f>& points, const size_t ptsToSpawn)
{
		const double radiusIncrement = 0.02 * bounds.height;

		size_t l = 0;
		do
		{
			l++;

			// E
			points.push_back(centerPoint + cv::Point2f(l * radiusIncrement, 0));
			// W
			points.push_back(centerPoint + cv::Point2f(-radiusIncrement * l, 0));
			// N
			points.push_back(centerPoint + cv::Point2f(0, radiusIncrement * l));
			// S
			points.push_back(centerPoint + cv::Point2f(0, -radiusIncrement * l));

			// NE
			points.push_back(centerPoint + cv::Point2f(2.0 * l * radiusIncrement / 3.0, 2.0 * l * radiusIncrement / 3.0));
			// NW
			points.push_back(centerPoint + cv::Point2f(-radiusIncrement * 2.0 * l / 3.0, 2.0 * l * radiusIncrement / 3.0));
			// SE
			points.push_back(centerPoint + cv::Point2f(radiusIncrement * 2.0 * l / 3.0, -radiusIncrement * 2.0 * l / 3.0));
			// SW
			points.push_back(centerPoint + cv::Point2f(-radiusIncrement * 2.0 * l / 3.0, -radiusIncrement * 2.0 * l / 3.0));

		} while (l * 8 < ptsToSpawn);
}

void lk::PointAccountant::replenish (std::vector<cv::Point2f>& points)
{
	if (points.empty())
	{
		spawn(prevCenter, points, idealPoints);
	}
	else if (points.size() < idealPoints)
	{
		// TODO this loop should be over ordering by most moved
		size_t k = 0;
		while (points.size() < idealPoints)
		{
			spawn(points[k], points, 8);
			k++;
		}	
	}
	// else do nothing
}

void lk::PointAccountant::weed (lk::LKData& data, const bool respawn)
{
	// refuse to move if outside bounds
	cv::Point2f nextCenter = getCenter(data.nextPts);

	if (withinBounds(nextCenter))
	{
		prevCenter = nextCenter;
	}

	// collapse the stack
	size_t i, k, oldSize = data.nextPts.size();
	for(i = k = 0; i < oldSize; i++)
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
		replenish(data.nextPts);
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

const cv::Size& lk::PointAccountant::getBounds () const
{
	return bounds;
}

const cv::Point2f& lk::PointAccountant::getPrevCenter () const
{
	return prevCenter;
}

bool lk::PointAccountant::withinBounds (const cv::Point2f& pt)
{
	return pt.x >= 0 && 
			pt.y >= 0 &&
			pt.x < bounds.width &&
			pt.y < bounds.height;
}
