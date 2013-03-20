#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <opencv2/highgui/highgui.hpp>
#include "HandTracker.hpp"
#include "BgSoundGenerator.hpp"
#include "NoteProvider.hpp"
#include "VideoProvider.hpp"

using namespace lk;

int main (int argc, char** argv)
{
//	const std::string mov = "resources/test.mov";
	VideoProvider vid;
	HandTracker tracker(vid.getFrameSize());

	NoteProvider np(0, vid.getFrameSize().height);

	std::vector<float> locs (np.getNoteLocations());

	cv::Mat frame;
	unsigned char key = 0;

	BgSoundGenerator bgSoundGen;
	bgSoundGen.start();

	int frameCount = 0;
	bool nightMode = false;

	while (key != 'q') 
	{
		// Check for stop
		key = cv::waitKey(10);

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
		std::for_each(locs.begin(), locs.end(), [&](float f){ 
			 cv::line(frame, cv::Point2f(0, f), cv::Point2f(frame.cols, f), cv::Scalar(200,200,100), 2);
		 });

		// Draw cursor
		cv::circle(frame, pos, 16, cv::Scalar(255,0,0), 3, 8);

		// Display
		cv::imshow("LK", frame);

		frameCount++;
	}

	bgSoundGen.stop();

	return EXIT_SUCCESS;
}
