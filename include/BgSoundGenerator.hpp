#ifndef BGSOUNDGENERATOR_H
#define BGSOUNDGENERATOR_H

#include <thread>
#include "SoundGenerator.hpp"

namespace lk
{
	class BgSoundGenerator
	{
		public:
			BgSoundGenerator ();
			void start ();
			void stop ();
			void setFrequency (float value);
		private:
			SoundGenerator soundGen;
			std::thread t;
	};
}

#endif
