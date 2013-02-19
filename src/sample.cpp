#include <string>
#include <iostream>
#include <vector>
#include <tuple>
#include <utility>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include "VideoProvider.hpp"

typedef std::pair<size_t,cv::Point> datum;
typedef std::vector<datum> dataVec;

static bool pause = false;
static dataVec clickData;
static size_t frameCount = 0;

// Pushes a new click event onto the clickData
static void mouseClick (int event, int x, int y, int, void*)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		cv::Point point(x, y);
		clickData.push_back(datum(frameCount, point));
		pause = false;
	}
}

// Fills dataOut with linear interpolation of dataIn's points
static void interpolate(const dataVec& dataIn, dataVec& dataOut)
{

	dataOut.resize(dataIn.size() * 10);

	// TODO - implement interpolation
	std::transform
	(
	 	begin(dataIn),
		end(dataIn),
		begin(dataOut),
		[](const datum& d) 
		{ 
			return d; 
		}
	);
}

int main (int argc, char** argv)
{
	std::string sampleFile = "resources/test.mov";
	lk::VideoProvider vid(sampleFile);

	cv::namedWindow(sampleFile, 0);
	cv::setMouseCallback(sampleFile, mouseClick, 0);

	bool play = true;
	cv::Mat frame;

	while (play)
	{
		for (size_t i = 0; i < 10 && !pause; i++)
		{
			// Read video
			if (!vid.isOpen())
			{
				play = false;
				break;
			}
			frame = vid.getFrame();
			frameCount++;
			cv::imshow(sampleFile, frame);
		}

		// skip reading whilst waiting for user input
		pause = true;
		cv::waitKey(10);
	}

	dataVec interpolatedData;
	interpolate(clickData, interpolatedData);

	std::for_each
	(	
	 	begin(interpolatedData),
		end(interpolatedData),
		[](datum& d) { std::cout << std::get<1>(d) << std::endl; }
	);

	return EXIT_SUCCESS;
}
