#include "SoundGenerator.hpp"

std::ostream& operator<<(std::ostream& ostr, lk::SoundGenerator soundGen)
{
	return ostr << "SoundGenerator [frequency=" << soundGen.getFrequency() << ", play=" << soundGen.getPlay() << "]" << std::endl;
}

lk::SoundGenerator::SoundGenerator () :
	frequency(440.0)
{
	stk::Stk::setSampleRate(44100.0);
	stk::Stk::showWarnings(true);
	sineWave.setFrequency(frequency);
	play.store(false);
}

const float lk::SoundGenerator::getFrequency () const
{
	return frequency.load();
}

void lk::SoundGenerator::setFrequency (float value)
{
	std::lock_guard<std::mutex> lock(mutex);
	frequency.store(value);
	sineWave.setFrequency(value);
}

const bool lk::SoundGenerator::getPlay () const
{
	return play.load();
}

void lk::SoundGenerator::setPlay (bool value)
{
	play.store(value);
}

void lk::SoundGenerator::start ()
{
	play.store(true);
	try
	{
		realTimeOutput = std::shared_ptr<stk::RtWvOut>(new stk::RtWvOut(1));

		while (play.load())
		{
			for (size_t i = 0; i < 1000; i++)
			{
				realTimeOutput->tick(sineWave.tick());
			}
		}
	}
	catch (stk::StkError&)
	{
		std::cout << "Failed to open real time wave out." << std::endl;
		exit(EXIT_FAILURE);
	}
}
