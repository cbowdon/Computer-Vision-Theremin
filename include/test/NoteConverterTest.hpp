#ifndef NOTECONVERTERTEST_H
#define NOTECONVERTERTEST_H

#include <cassert>
#include "TestBase.hpp"
#include "NoteGenerator.hpp"

namespace test
{
	class NoteGeneratorTest : public TestBase
	{
		public:
			bool run () override;
			const std::string getName () const override;
		private:
			const bool getNameForInterval () const;
	};
}

#endif
