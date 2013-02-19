#ifndef NOTECONVERTER_H
#define NOTECONVERTER_H

#include <cmath>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <string>
#include <sstream>
#include "LinearConverter.hpp"

namespace lk 
{
	class NoteGenerator
	{
		public:

			NoteGenerator () = delete;
			NoteGenerator (NoteGenerator& nc) = delete;
			NoteGenerator operator=(NoteGenerator& nc) = delete;

			NoteGenerator (float min, float max);

			const float convert (float value) const;

			const std::vector<float>& getNoteLocations ();

			const size_t getIntervalForName (std::string name) const;
			const std::string getNameForInterval (size_t interval) const;

		private:

			// C0 = 440Hz / exp((57 / 12) * log(2))
			const double C0 = 16.3515978312874;
			const std::vector<std::string> names { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

			LinearConverter lc;
			
			const size_t noteMin;
			const size_t noteMax;
			const size_t noteDist;

			std::vector<float> locations;

			const float getFrequencyForInterval (float interval) const;
			const float stickyInt (float value) const;
	};
}

#endif
