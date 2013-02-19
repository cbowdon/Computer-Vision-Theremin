#include "LinearConverterTest.hpp"

const std::string test::LinearConverterTest::getName () const
{
	return "LinearConverter Test";
}

bool test::LinearConverterTest::run ()
{
	bool positiveCalcsResult = positiveCalcs();
	bool negativeCalcsResult = negativeCalcs();
	bool crossOriginCalcsResult = crossOriginCalcs();
	bool edgeCaseCalcsResult = edgeCaseCalcs();

	std::cout << "\t" << "positiveCalcs:\t" << positiveCalcsResult << std::endl;
	std::cout << "\t" << "negativeCalcs:\t" << negativeCalcsResult << std::endl;
	std::cout << "\t" << "crossOriginCalcs:\t" << crossOriginCalcsResult << std::endl;
	std::cout << "\t" << "edgeCaseCalcs:\t" << edgeCaseCalcsResult << std::endl;

	return positiveCalcsResult && negativeCalcsResult && crossOriginCalcsResult && edgeCaseCalcsResult;
}

const bool test::LinearConverterTest::positiveCalcs () const
{
	lk::LinearConverter lc;
	lc.setXRange(0.0, 1.0);
	lc.setYRange(1.0, 3.0); // y = 2 x + 1

	bool success = lc.convertXToY(0.25) == 1.5;
	assert(success);

	success &= lc.convertXToY(0) == 1.0;
	assert(success);

	lc.setYRange(2.0, 6.0); // y = 4 x + 2

	success &= lc.convertXToY(0.25) == 3.0;
	assert(success);

	return success;
}

const bool test::LinearConverterTest::negativeCalcs () const
{
	lk::LinearConverter lc;
	lc.setXRange(-1.0, 0.0);
	lc.setYRange(1.0, 3.0); // y = 2 x + 3

	bool success = lc.convertXToY(0.25) == 3.5;
	assert(success);

	success &= lc.convertXToY(0) == 3.0;
	assert(success);

	lc.setXRange(-4.0, -1.0);
	lc.setYRange(-3.0, 0.0); // y = x + 1

	success = lc.convertXToY(0.25) == 1.25;
	assert(success);

	return success;
}

const bool test::LinearConverterTest::crossOriginCalcs () const
{
	lk::LinearConverter lc;
	lc.setXRange(-1.0, 1.0);
	lc.setYRange(1.0, 3.0); // y = x + 2

	bool success = lc.convertXToY(0.25) == 2.25;
	assert(success);

	success &= lc.convertXToY(0) == 2.0;
	assert(success);

	lc.setXRange(-1.0, 4.0);
	lc.setYRange(-6.0, 4.0); // y = 2 x - 4;

	success = lc.convertXToY(0.25) == -3.5;
	assert(success);

	return success;
}

const bool test::LinearConverterTest::edgeCaseCalcs () const
{
	lk::LinearConverter lc;

	bool success = lc.convertXToY(0.0) == 0.0;
	assert(success);

	return success;
}
