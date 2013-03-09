#include "HandTracker.hpp"

lk::HandTracker::HandTracker (const cv::Size& frameSize) :
	termCrit(cv::TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03)),
	winSize(cv::Size(21, 21)),
	frameSize(frameSize),
	pa(frameSize, 16, true)
{
	pa.replenish(data.prevPts);
	pa.replenish(data.nextPts);
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
		pa.replenish(data.nextPts);
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

	pa.weed(data);

	std::swap(data.prevPts, data.nextPts);
	std::swap(prevImageLK, gray);
}

std::ostream& operator<<(std::ostream& ostr, const lk::HandTracker& ht)
{
	return ostr << "HandTracker: [pos = " << ht.getPos() << "]";
}
