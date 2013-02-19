#include "Note.hpp"

std::ostream& operator<< (std::ostream& ostr, const lk::Note note)
{
	ostr << "Note [name=" << note.name 
		<< ", interval=" << note.interval 
		<< ", frequency=" << note.frequency << "Hz]";
	return ostr;
}

lk::Note::Note (size_t interval) :
	interval(interval),
	semitone(interval % 12),
	octave((interval - semitone) / 12),
	name(getNameForInterval(interval)),
	frequency(getFrequencyForInterval(interval))
{
}

const float lk::Note::getFrequencyForInterval (size_t interval) const
{
	return C0 * exp((interval / 12.0) * log(2.0));
}

const std::string lk::Note::getNameForInterval (size_t interval) const
{
	std::stringstream ss;
	ss << names[semitone] << octave;
	return ss.str();
}
