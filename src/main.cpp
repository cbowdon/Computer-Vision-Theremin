#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include <opencv2/highgui/highgui.hpp>
#include <boost/timer/timer.hpp>

#include "VideoProvider.hpp"
#include "Profiler.hpp"
#include "HandTracker.hpp"
#include "BgSoundGenerator.hpp"
#include "NoteProvider.hpp"

int main (int argc, char** argv)
{
	const std::string videoFile = "resources/test.mov";
	lk::VideoProvider vid;
	lk::Profiler profiler;
	lk::HandTracker tracker(vid.getFrameSize());

	lk::NoteProvider np(0, vid.getFrameSize().width);

	std::vector<float> locs (np.getNoteLocations());

	cv::Mat frame;
	unsigned char key = 0;

	boost::timer::auto_cpu_timer autoCPUTimer;

	lk::BgSoundGenerator bgSoundGen;

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
		bgSoundGen.setFrequency(np.getNote(pos.x).frequency);

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
				cv::line(frame, cv::Point2f(f, 0), cv::Point2f(f, frame.rows), cv::Scalar(200,200,100), 2);
			}
		);

		// Draw cursor
		cv::circle(frame, pos, 16, cv::Scalar(255,0,0), 3, 8);
		cv::circle(frame, cv::Point2f(pos.x, frame.rows / 2), 16, cv::Scalar(100,200,200), 3, 8);

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
}
