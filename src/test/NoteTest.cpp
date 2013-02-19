#include "NoteTest.hpp"

const std::string test::NoteTest::getName () const
{
	return "Note Test";
}

bool test::NoteTest::run ()
{
	bool createResult = create();
	std::cout << "\t" << "create:\t" << createResult << std::endl;
	return createResult;
}

const bool test::NoteTest::create () const
{
	lk::Note myC0(0);
	
	bool success = myC0.name == "C0";
	assert(success);

	success &= myC0.frequency == static_cast<float>(myC0.C0);
	assert(success);

	lk::Note myC1(12);

	success &= myC1.name == "C1";
	assert(success);

	success &= myC1.frequency == myC0.frequency * 2;
	assert(success);

	lk::Note myA4(57);
	success &= myA4.name == "A4";
	assert(success);

	success &= myA4.frequency == 440.0;
	assert(success);

	return success;
}
