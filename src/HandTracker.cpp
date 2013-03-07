#include "HandTracker.hpp"

lk::HandTracker::HandTracker (const cv::Size& frameSize) :
	termCrit(cv::TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03)),
	winSize(cv::Size(31, 31)),
	frameSize(frameSize),
	pa(frameSize, 10, true)
{
	pa.spawnPoints(data.prevPts);
	pa.spawnPoints(data.nextPts);
}

const cv::Point& lk::HandTracker::getPos () const
{
	return pos;
}

void lk::HandTracker::sharpen (const cv::Mat& in, cv::Mat& out) const
{
	cv::GaussianBlur(in, out, cv::Size(0, 0), 3);
	cv::addWeighted(in, 1.5, out, -0.5, 0, out);
}

void lk::HandTracker::update (cv::Mat& frame, bool shouldSharpen)
{
	cv::Mat gray, sharp;
	cv::cvtColor(frame, gray, CV_BGR2GRAY);

	if (shouldSharpen)
	{
		cv::Mat temp;
		sharpen(gray, temp);
		gray = temp;
	}

	if (!prevImageLK.data)
	{
		gray.copyTo(prevImageLK);
	}

	if (data.nextPts.empty())
	{
		pa.spawnPoints(data.nextPts);
	}

	if (!data.prevPts.empty() && !data.nextPts.empty())
	{
		cv::calcOpticalFlowPyrLK(prevImageLK, gray, data.prevPts, data.nextPts, data.status, data.error, winSize, 3, termCrit, 0, 0.001);
	}

	for (auto& pt : data.prevPts)
	{
		cv::circle(frame, pt, 3, cv::Scalar(0,255,0), -1, 8);
	}

	pos = pa.getPrevCenter();
	cv::circle(frame, pos, 3, cv::Scalar(0,0,255), -1, 8);

	stats.push_back(pa.getDataStats(data));

	pa.weedPoints(data);

	std::swap(data.prevPts, data.nextPts);
	std::swap(prevImageLK, gray);
}

const std::string lk::HandTracker::report () const
{
	cv::Point2f range(0, 0);
	cv::Point2f maxRange(0, 0);

	for (auto& stat : stats)
	{
		range += stat.range;
		if (stat.range.x > maxRange.x)
		{
			maxRange.x = stat.range.x;
		}
		if (stat.range.y > maxRange.y)
		{
			maxRange.y = stat.range.y;
		}
	}
	range.x /= stats.size();
	range.y /= stats.size();

	float h = 100.0 / frameSize.width;
	float w = 100.0 / frameSize.height;

	range.x *= w;
	range.y *= h;
	maxRange.x *= w;
	maxRange.y *= h;

	std::stringstream ss;
	ss << "HandTracker report:" << std::endl;
	ss << "\tStart:\t" << stats[0].center << std::endl;
	size_t last = stats.size() - 2;
	ss << "\tFinish:\t" << stats[last].center << std::endl;
	ss << "\tAverage range (%):\t" << range << std::endl;
	ss << "\tMax range (%):\t" << maxRange << std::endl;
	return ss.str();
}

std::ostream& operator<<(std::ostream& ostr, const lk::HandTracker& ht)
{
	return ostr << "HandTracker: [pos = " << ht.getPos() << "]";
}
