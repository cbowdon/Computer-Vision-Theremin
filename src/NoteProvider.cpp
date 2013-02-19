#include "NoteProvider.hpp"

lk::NoteProvider::NoteProvider (float min, float max) :
	noteMin(48),
	noteMax(60),
	noteDist(noteMax - noteMin)
{
	lc.setXRange(min, max);
	lc.setYRange(noteMin, noteMax);

	std::vector<size_t> range(noteDist);

	std::iota(range.begin(), range.end(), noteMin);

	std::for_each(range.begin(), range.end(), [&](size_t i) { notes.push_back(Note(i)); });
}

const lk::Note& lk::NoteProvider::getNote (float value) const
{
	size_t interval = static_cast<size_t>(lc.convertXToY(value)) - noteMin;

	if (interval >= noteDist)
	{
		interval = noteDist - 1;
	}

	return notes[interval];
}

const std::vector<float>& lk::NoteProvider::getNoteLocations ()
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

