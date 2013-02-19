#ifndef NOTETEST_H
#define NOTETEST_H

#include <string>
#include <iostream>
#include <cassert>
#include "TestBase.hpp"
#include "Note.hpp"

namespace test
{
	class NoteTest : public TestBase
	{
		public:
			bool run () override;
			const std::string getName() const override;
		private:
			const bool create () const;
	};
}

#endif
