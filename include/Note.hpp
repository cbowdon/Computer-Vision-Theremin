#ifndef NOTE_H
#define NOTE_H

#include <cmath>
#include <string>
#include <ostream>
#include <sstream>
#include <vector>

namespace lk
{
	struct Note
	{
		const double C0 = 16.3515978312874;
		const std::vector<std::string> names { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

		const size_t interval;
		const size_t semitone;
		const size_t octave;

		const std::string name;
		const float frequency;

		Note (size_t interval);

		Note () = delete;
		Note (const Note& note) = default;
		Note& operator= (const Note& note) = delete;

		private:

			const float getFrequencyForInterval (size_t interval) const;
			const std::string getNameForInterval (size_t interval) const;
	};
}

extern std::ostream& operator<< (std::ostream& ostr, const lk::Note note);

#endif
