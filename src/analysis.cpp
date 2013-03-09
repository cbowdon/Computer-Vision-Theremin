#include <iostream>
#include <iomanip>
#include <boost/timer/timer.hpp>
#include <opencv2/core/core.hpp>
#include "statistics.hpp"
#include "TrackFile.hpp"
#include "VideoProvider.hpp"
#include "Profiler.hpp"
#include "HandTracker.hpp"

using namespace lk;
using namespace statistics;

int main (int argc, char** argv)
{

	// Run video clip
	boost::timer::auto_cpu_timer autoCPUTimer;

	const std::string videoFile = "resources/test.mov";

	VideoProvider vid(videoFile);

	Profiler profiler;

	HandTracker tracker(vid.getFrameSize());

	std::stringstream ss;
	ss << "resources/test" << time(0) << ".txt";
	TrackFile trackFile(ss.str());

	std::vector<trackDatum> record;

	cv::Mat frame;
	size_t frameCount = 0;
	while(true)
	{
		profiler.setCheckPoint();

		// Read video
		if (!vid.isOpen())
		{
			break;
		}

		frame = vid.getFrame();

		// Track hand
		cv::Point2f pos = tracker.getPos();

		tracker.update(frame);

		record.push_back(trackDatum(frameCount, cv::Point2d(pos.x, pos.y)));

		// Profile
		float fps = 1.0 / profiler.getElapsedSeconds();
		if (frameCount % 20 == 0)
		{
			std::cout << std::setw(6) << fps << " fps" << std::endl;
		}
		frameCount++;
	}

	// Process the data
	std::vector<trackDatum> human;
	TrackFile humanFile("resources/human.txt");
	humanFile.load(human);

	std::vector<trackDatum> rec2(frameCount);
	std::transform(begin(record), end(record), begin(rec2), [](trackDatum d){
			double x = std::get<1>(d).x;
			double y = std::get<1>(d).y;
			cv::Point2d p2(x - 20, y - 30);
			return trackDatum(std::get<0>(d), p2);
			});

	double chiX = chiSquared(begin(human), end(human), begin(rec2), [](const trackDatum d){ return std::get<1>(d).x; });
	double chiY = chiSquared(begin(human), end(human), begin(rec2), [](const trackDatum d){ return std::get<1>(d).y; });

	std::cout << chiX << std::endl;
	std::cout << chiY << std::endl;

	std::cout << human.size() << std::endl;

	frame = cv::Scalar::all(0);
	for (;;)
	{
		if (cv::waitKey(5) == 'q')
		{
			break;
		}

		std::for_each(begin(human), end(human), [&frame](trackDatum d){
				cv::circle(frame, std::get<1>(d), 4, cv::Scalar(255,0,0), 3, 8);
				});

		std::for_each(begin(rec2), end(rec2), [&frame](trackDatum d){
				cv::circle(frame, std::get<1>(d), 4, cv::Scalar(0,255,0), 3, 8);
				});

		cv::imshow("LK", frame);
	}

	return EXIT_SUCCESS;
}
