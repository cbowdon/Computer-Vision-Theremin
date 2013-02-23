#include <string>
#include <ostream>
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <utility>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include "VideoProvider.hpp"

typedef std::pair<size_t,cv::Point2f> datum;
typedef std::vector<datum> dataVec;

std::ostream& operator<<(std::ostream& ostr, const datum d)
{
	ostr << std::get<0>(d) << ", " << std::get<1>(d);
	return ostr;
}

static bool pause = false;
static dataVec clickData;
static size_t frameCount = 0;

// Pushes a new click event onto the clickData
static void mouseClick (int event, int x, int y, int, void*)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		cv::Point2f point(x, y);
		clickData.push_back(datum(frameCount, point));
		pause = false;
	}
}

// Fills dataOut with linear interpolation of dataIn's points
static void lerp(const dataVec& dataIn, dataVec& dataOut)
{

//	dataOut.resize(dataIn.size() * 10);

	dataVec::const_iterator it = begin(dataIn), nd = end(dataIn);

	float fc0 = 0, fc1 = 0;
	cv::Point2f pt0, pt1;
	for (it = begin(dataIn); it != nd; it++)
	{
		fc1 = std::get<0>(*it);
		pt1 = std::get<1>(*it);

		float nFrame = fc1 - fc0;

		float xDist = pt1.x - pt0.x;
		float yDist = pt1.y - pt0.y;

		float xIncrement = xDist / nFrame;
		float yIncrement = yDist / nFrame;

		for (size_t i = 0; i < nFrame; i++)
		{
			cv::Point2f interPt(i * xIncrement + pt0.x, i * yIncrement + pt0.y);
			dataOut.push_back(datum(fc0 + i, interPt));
		}

		std::swap(fc0, fc1);
		std::swap(pt0, pt1);
	}
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

	dataVec lerpdData;
	lerp(clickData, lerpdData);

	std::for_each
	(	
	 	begin(lerpdData),
		end(lerpdData),
		[&frame](datum& d) 
		{ 
			cv::circle(frame, std::get<1>(d), 2, cv::Scalar(255,0,0), 3, 8);
//			std::cout << std::get<1>(d) << std::endl; 
			std::cout << d << std::endl;
		}
	);
	bool quit = false;
	while (!quit)
	{
		cv::imshow(sampleFile, frame);
		quit = cv::waitKey(5) == 'q';
	}

	return EXIT_SUCCESS;
}
