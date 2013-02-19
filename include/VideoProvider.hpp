#ifndef VIDEOPROVIDER_H
#define VIDEOPROVIDER_H

#include <string>
#include <ostream>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace lk
{
	class VideoProvider
	{
		public:
			VideoProvider (bool mirror=true);
			VideoProvider (const std::string videoFile, bool mirror=true);
			cv::Mat getFrame ();
			const bool isOpen () const;
			const cv::Size& getFrameSize () const;

		private:
			void buildMirrorMap ();
			void mirror (cv::Mat& frame);

			VideoProvider& operator=(VideoProvider vp) = delete;
			VideoProvider (const VideoProvider& vp) = delete;

			cv::VideoCapture cap;
			bool shouldMirror;
			const size_t timeout;
			const bool usingVideoFile;
			cv::Size frameSize;
			cv::Mat mirrorMapX;
			cv::Mat mirrorMapY;
	};
}

extern std::ostream& operator<<(std::ostream& ostr, const lk::VideoProvider& vp);
#endif
