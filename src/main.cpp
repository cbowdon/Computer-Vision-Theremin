#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <opencv2/highgui/highgui.hpp>
#include <boost/timer/timer.hpp>
#include "statistics.hpp"
#include "TrackFile.hpp"
#include "HandTracker.hpp"
#include "BgSoundGenerator.hpp"
#include "NoteProvider.hpp"
#include "VideoProvider.hpp"
#include "Profiler.hpp"

using namespace lk;
using namespace statistics;

int main (int argc, char** argv)
{
	const std::string videoFile = "resources/test.mov";
	VideoProvider vid(videoFile);
	Profiler profiler;
	HandTracker tracker(vid.getFrameSize());

	std::stringstream ss;
	ss << "resources/test" << time(0) << ".txt";
	TrackFile trackFile(ss.str());

	NoteProvider np(0, vid.getFrameSize().height);

	std::vector<float> locs (np.getNoteLocations());

	std::vector<trackDatum> record;

	cv::Mat frame;
	unsigned char key = 0;

	boost::timer::auto_cpu_timer autoCPUTimer;

	BgSoundGenerator bgSoundGen;
	bgSoundGen.start();

	int frameCount = 0;
	bool nightMode = false;

	while (key != 'q') 
	{
		// Check for stop
		key = cv::waitKey(10);

		profiler.setCheckPoint();

		// Read video
		if (!vid.isOpen())
		{
			break;
		}
		frame = vid.getFrame();

		// Calculate hand position
		cv::Point2f pos = tracker.getPos();
		tracker.update(frame);

		// Generate sound
		bgSoundGen.setFrequency(np.getNote(frame.rows - pos.y).frequency);

		// Night mode!
		if (key == 't')
		{
			nightMode = !nightMode;
		}
		if (nightMode)
		{
			frame = cv::Scalar::all(0);
		}

		// Draw nc divisions
		std::for_each
			(	
			 locs.begin(), 
			 locs.end(), 
			 [&](float f)
			 { 
			 cv::line(frame, cv::Point2f(0, f), cv::Point2f(frame.cols, f), cv::Scalar(200,200,100), 2);
			 }
			);

		// Draw cursor
		cv::circle(frame, pos, 16, cv::Scalar(255,0,0), 3, 8);
		// Draw left hand side cursor
		cv::circle(frame, cv::Point2f(frame.cols / 20, pos.y), 16, cv::Scalar(100,200,200), 3, 8);

		record.push_back(trackDatum(frameCount, cv::Point2d(pos.x, pos.y)));


		// Display
		cv::imshow("LK", frame);

		// Profile
		float fps = 1.0 / profiler.getElapsedSeconds();
		if (frameCount % 20 == 0)
		{
			std::cout << fps << " fps" << std::endl;
		}
		frameCount++;
	}

	bgSoundGen.stop();

	std::cout << tracker.report() << std::endl;

	trackFile.save(record);

	std::vector<trackDatum> human;
	TrackFile humanFile("resources/human.txt");
	humanFile.load(human);

	std::vector<trackDatum> record20(record.size());
	std::transform(begin(record), end(record), begin(record20), [](trackDatum d){ 
			cv::Point2d p20(std::get<1>(d).x, std::get<1>(d).y+10.0); 
			return trackDatum(std::get<0>(d), p20);
		});

	double chiX = chiSquared(begin(human), end(human), begin(record), [](trackDatum d){ return std::get<1>(d).x; });
	double chiY = chiSquared(begin(human), end(human), begin(record20), [](trackDatum d){ return std::get<1>(d).y; });

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

		std::for_each(begin(human), 
				end(human), 
				[&frame](trackDatum d)
				{
					cv::circle(frame, std::get<1>(d), 4, cv::Scalar(255,0,0), 3, 8);
				});

		std::for_each(begin(record), 
				end(record), 
				[&frame](trackDatum d)
				{
					cv::circle(frame, std::get<1>(d), 4, cv::Scalar(0,255,0), 3, 8);
				});

		cv::imshow("LK", frame);
	}

	return EXIT_SUCCESS;
}
