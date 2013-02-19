#include "NoteGeneratorTest.hpp"

const std::string test::NoteGeneratorTest::getName () const
{
	return "NoteGenerator Test";
}

bool test::NoteGeneratorTest::run ()
{
	bool getNameForIntervalResult = getNameForInterval();
	std::cout << "\t" << "getNameForInterval:\t" << getNameForIntervalResult << std::endl;
	return getNameForIntervalResult;
}

const bool test::NoteGeneratorTest::getNameForInterval () const
{
	lk::NoteGenerator nc(0, 100);

	bool success = nc.getNameForInterval(0) == "C0";
	assert(success);
	success &= nc.getNameForInterval(12) == "C1";
	assert(success);
	success &= nc.getNameForInterval(13) == "C#1";
	assert(success);

	return success;
}
