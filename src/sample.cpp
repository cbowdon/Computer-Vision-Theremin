#include <string>
#include <ostream>
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <utility>
#include <algorithm>
#include <numeric>
#include <opencv2/core/core.hpp>
#include "interpolation.hpp"
#include "VideoProvider.hpp"
#include "TrackFile.hpp"

typedef std::vector<lk::trackDatum> dataVec;

static bool pause = false;
static dataVec clickData;
static size_t frameCount = 0;

// Pushes a new click event onto the clickData
static void mouseClick (int event, int x, int y, int, void*)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		cv::Point2d point(x, y);
		clickData.push_back(lk::trackDatum(frameCount, point));
		pause = false;
	}
}

static void interpolate (const dataVec& dataIn, dataVec& dataOut)
{
	const size_t nSteps = 10;
	const size_t interpolatedSize = (dataIn.size() - 1) * nSteps;
	dataOut.resize(interpolatedSize);

	std::vector<double> x(dataIn.size()), y(dataIn.size());
	std::transform(begin(dataIn), end(dataIn), begin(x), [](lk::trackDatum d){ return std::get<1>(d).x; });
	std::transform(begin(dataIn), end(dataIn), begin(y), [](lk::trackDatum d){ return std::get<1>(d).y; });

	std::vector<size_t> frameIndices(interpolatedSize);
	std::iota(begin(frameIndices), end(frameIndices), 0);

	std::vector<double> x2(interpolatedSize), y2(interpolatedSize);
	interpolation::cubic(begin(x), end(x), begin(x2), nSteps);
	interpolation::cubic(begin(y), end(y), begin(y2), nSteps);

	for (size_t i = 0; i < interpolatedSize; i++)
	{
		dataOut[i] = lk::trackDatum(frameIndices[i], cv::Point2d(x2[i], y2[i]));
	}
}

int main (int argc, char** argv)
{
	std::string sampleFile = "resources/test.mov";
	lk::VideoProvider vid(sampleFile);
	lk::TrackFile trackFile("resources/test.txt");

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
		[&frame](lk::trackDatum& d) 
		{ 
			cv::circle(frame, std::get<1>(d), 2, cv::Scalar(255,0,0), 3, 8);
		}
	);

	trackFile.save(interpolatedData);

	bool quit = false;
	while (!quit)
	{
		cv::imshow(sampleFile, frame);
		quit = cv::waitKey(5) == 'q';
	}

	return EXIT_SUCCESS;
}
