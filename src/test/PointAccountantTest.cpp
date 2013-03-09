#include "PointAccountantTest.hpp"

std::ostream& operator<<(std::ostream& ostr, const test::PointAccountantTest pat)
{
	return ostr << "PointAccountant Test";
}

const std::string test::PointAccountantTest::getName () const
{
	return "PointAccountant Test";
}

bool test::PointAccountantTest::run ()
{
	bool replenishResult = replenish();
	bool weedResult = weed();
	bool getCenterResult = getCenter();

	std::cout << "\t" << "replenish:\t" << replenishResult << std::endl;
	std::cout << "\t" << "weed:\t" << weedResult << std::endl;
	std::cout << "\t" << "getCenter:\t" << getCenterResult << std::endl;

	return replenishResult && weedResult && getCenterResult;
}

bool test::PointAccountantTest::replenish ()
{
	bool success;
	const size_t idealPoints = 10;
	lk::PointAccountant pa(cv::Size(100, 100), idealPoints, false);

	// Init empty array
	std::vector<cv::Point2f> points;

	// spawn initial 10 points
	pa.replenish(points);
	success = points.size() >= idealPoints;
	assert(success);

	// immediate re-spawn does nothing
	const size_t currentPts = points.size();
	pa.replenish(points);
	success &= points.size() == currentPts;
	assert(success);

	// re-spawn after loss rebuilds pointer
	points.resize(5);
	pa.replenish(points);
	success &= points.size() >= idealPoints;
	assert(success);

	return success;
}

bool test::PointAccountantTest::weed ()
{
	lk::LKData data;
    
	data.prevPts.push_back(cv::Point2f(10, 10));
	data.prevPts.push_back(cv::Point2f(10, 10));
	data.prevPts.push_back(cv::Point2f(10, 10));
	data.prevPts.push_back(cv::Point2f(10, 10));
	data.prevPts.push_back(cv::Point2f(10, 10));

	data.nextPts.push_back(cv::Point2f(20, 20));
	data.nextPts.push_back(cv::Point2f(15, 15));
	data.nextPts.push_back(cv::Point2f(10, 10));
	data.nextPts.push_back(cv::Point2f(8, 8));
	data.nextPts.push_back(cv::Point2f(5, 5));

	data.status.push_back(1);
	data.status.push_back(1);
	data.status.push_back(1);
	data.status.push_back(0);
	data.status.push_back(1);

	data.error.push_back(0);
	data.error.push_back(0);
	data.error.push_back(0);
	data.error.push_back(0);
	data.error.push_back(100);

	lk::PointAccountant pa(cv::Size(100, 100), 5, true);
	pa.weed(data, false);

	bool success;
	success = data.nextPts.size() == 2;
	assert(success);

	return success;
}

bool test::PointAccountantTest::getCenter ()
{
	bool success;
	lk::PointAccountant pa(cv::Size(100, 100), 5, false);

	std::vector<cv::Point2f> points;

	points.push_back(cv::Point2f(0,0));
	points.push_back(cv::Point2f(2,2));

	success = pa.getCenter(points) == cv::Point2f(1,1);
	assert(success);

	points.push_back(cv::Point2f(0,2));
	points.push_back(cv::Point2f(2,0));

	success &= pa.getCenter(points) == cv::Point2f(1,1);
	assert(success);

	points.push_back(cv::Point2f(5,0));
	points.push_back(cv::Point2f(0,2));

	success &= pa.getCenter(points) == cv::Point2f(1.5,1);
	assert(success);

	return success;
}
