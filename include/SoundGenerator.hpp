#ifndef SOUNDGENERATOR_H
#define SOUNDGENERATOR_H

#include <iostream>
#include <thread>
#include <atomic>
#include <stk/SineWave.h>
#include <stk/RtWvOut.h>

namespace lk
{

	class SoundGenerator
	{
		public:
			SoundGenerator ();
			void start ();
			const float getFrequency () const;
			void setFrequency (float value);
			const bool getPlay () const;
			void setPlay (bool value);
		private:
			std::atomic<float> frequency;
			std::atomic<bool> play;
			std::mutex mutex;
			stk::SineWave sineWave;
			std::shared_ptr<stk::RtWvOut> realTimeOutput;
	};
}

extern std::ostream& operator<<(std::ostream& ostr, lk::SoundGenerator soundGen);

#endif
