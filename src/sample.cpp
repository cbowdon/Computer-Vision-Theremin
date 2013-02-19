#include <string>
#include <opencv2/core/core.hpp>
#include "VideoProvider.hpp"

int main (int argc, char** argv)
{
	std::string sampleFile = "resources/test.mov";
	lk::VideoProvider vid(sampleFile);

	bool play = false;
	cv::Mat frame;
	while (play)
	{
		for (size_t i = 0; i < 10; i++)
		{
			// Read video
			if (!vid.isOpen())
			{
				break;
			}
			frame = vid.getFrame();
		}
	}
}
