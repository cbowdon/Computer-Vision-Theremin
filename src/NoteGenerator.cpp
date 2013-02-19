#include "NoteGenerator.hpp"

lk::NoteGenerator::NoteGenerator (float min, float max) :
	noteMin(48),
	noteMax(60),
	noteDist(noteMax-noteMin)
{
	lc.setXRange(min, max);
	lc.setYRange(noteMin, noteMax);
}

const float lk::NoteGenerator::convert (float value) const
{
	float interval = lc.convertXToY(value);
	float freq = getFrequencyForInterval(interval);
	return freq < 1 ? 1 : freq;
}

const std::vector<float>& lk::NoteGenerator::getNoteLocations ()
{
	if (locations.empty())
	{
		std::vector<float> intervals(noteDist);
		std::iota(intervals.begin(), intervals.end(), noteMin);

		locations.resize(noteDist);

		std::transform
		(
			intervals.begin(), 
			intervals.end(), 
			locations.begin(), 
			[&](float i) 
			{
				return lc.convertYToX(i);
			}
		); 
	}

	return locations;
}

const float lk::NoteGenerator::getFrequencyForInterval (float value) const
{
//	std::cout << getNameForInterval(static_cast<int>(value)) << std::endl;
	return C0 * exp((std::floor(value) / 12.0) * log(2.0));
}

const float lk::NoteGenerator::stickyInt (float value) const
{
	int closest = std::round(value);
	return std::abs(value - closest) < 0.25 ? closest : value;
}

const std::string lk::NoteGenerator::getNameForInterval (size_t interval) const
{
	size_t semiTone = interval % 12;
	size_t octave = (interval - semiTone) / 12;
	std::stringstream ss;
	ss << names[semiTone] << octave;
	return ss.str();
}
