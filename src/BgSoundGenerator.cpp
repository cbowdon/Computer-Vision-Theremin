#include "BgSoundGenerator.hpp"

lk::BgSoundGenerator::BgSoundGenerator () 
{
	soundGen.setFrequency(440.0);
}

lk::BgSoundGenerator::~BgSoundGenerator ()
{
	try
	{
		if (t.joinable())
		{
			t.join();
		}
	}
	catch (...)
	{
	}
}

void lk::BgSoundGenerator::start ()
{
	t = std::thread(&SoundGenerator::start, &soundGen);
}

void lk::BgSoundGenerator::stop ()
{
	soundGen.setPlay(false);
	t.join();
}

void lk::BgSoundGenerator::setFrequency (float value)
{
	soundGen.setFrequency(value);
}
