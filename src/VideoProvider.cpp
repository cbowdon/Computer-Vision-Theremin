#include "VideoProvider.hpp"

lk::VideoProvider::VideoProvider (bool mirror) :
	shouldMirror(mirror),
	timeout(500), 
	usingVideoFile(false)
{
	std::cout << "Reading video from camera 0." << std::endl;
	cap.open(0);
	if (!cap.isOpened())
	{
		std::cout << "Failed to open video source!" << std::endl;
		exit(EXIT_FAILURE);
	}

	int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	frameSize = cv::Size(width, height);

	if (shouldMirror)
	{
		buildMirrorMap();
	}
}

lk::VideoProvider::VideoProvider (const std::string videoFile, bool mirror) :
	timeout(500),
	usingVideoFile(true)
{
	std::cout << "Reading video from " << videoFile << std::endl;
	cap.open(videoFile);
	if (!cap.isOpened())
	{
		std::cout << "Failed to open video source!" << std::endl;
		exit(EXIT_FAILURE);
	}

	int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	frameSize = cv::Size(width, height);

	if (shouldMirror)
	{
		buildMirrorMap();
	}
}

cv::Mat lk::VideoProvider::getFrame ()
{
	cv::Mat frame;
	size_t i = 0;
	while (!frame.data)
	{
		cap >> frame;

		if (i > timeout)
		{
			frame = cv::Mat::zeros(frameSize, CV_8UC3);
			if (usingVideoFile)
			{
				cap.release();
				break;
			}
			else
			{
				std::cout << "No more video input. Exiting." << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		i++;
	}
	if (shouldMirror)
	{
		mirror(frame);
	}
	return frame;
}

void lk::VideoProvider::buildMirrorMap ()
{
	mirrorMapX = cv::Mat(frameSize, CV_32F);
	mirrorMapY = cv::Mat(frameSize, CV_32F);

	for (int j = 0; j < frameSize.height; j++) {
		for (int i = 0; i < frameSize.width; i++) {
			mirrorMapX.at<float>(j, i) = frameSize.width - i;
			mirrorMapY.at<float>(j, i) = j;
		}
	}
}

void lk::VideoProvider::mirror (cv::Mat& frame)
{
	cv::Mat image;
	cv::remap(frame, image, mirrorMapX, mirrorMapY, CV_INTER_LINEAR);
	frame = image;
}

const bool lk::VideoProvider::isOpen () const
{
	return cap.isOpened();
}

const cv::Size& lk::VideoProvider::getFrameSize () const
{
	return frameSize;
}

std::ostream& operator<<(std::ostream& ostr, const lk::VideoProvider& vp)
{
	return ostr << "Video Provider: [isOpen() = " << vp.isOpen() << ", frameSize = " << vp.getFrameSize() << "]";
}
