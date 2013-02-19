#ifndef NOTEPROVIDER_H
#define NOTEPROVIDER_H

#include <numeric>
#include <algorithm>
#include <iostream>
#include "Note.hpp"
#include "LinearConverter.hpp"

namespace lk
{
	class NoteProvider
	{
		public:
			NoteProvider(float min, float max);
			const Note& getNote (float value) const;
			const std::vector<float>& getNoteLocations ();

		private:
			NoteProvider() = delete;
			NoteProvider(NoteProvider& np) = delete;
			NoteProvider operator=(NoteProvider& np) = delete;

			const size_t noteMin;
			const size_t noteMax;
			const size_t noteDist;

			lk::LinearConverter lc;
			std::vector<Note> notes;
			std::vector<float> locations;
	};
}

#endif
