#include "InterpolationTest.hpp"

const std::string test::InterpolationTest::getName () const
{
	return "Interpolation Test";
}

bool test::InterpolationTest::run ()
{
	bool linearResult = linear();
	bool smoothstepResult = smoothstep();
	bool lowpassResult = lowpass();
	bool cosineResult = cosine();
	bool splineResult = spline();
	std::cout << "\t" << "linear:\t" << linearResult << std::endl;
	std::cout << "\t" << "smoothstep:\t" << smoothstepResult << std::endl;
	std::cout << "\t" << "lowpass:\t" << lowpassResult << std::endl;
	std::cout << "\t" << "cosine:\t" << cosineResult << std::endl;
	std::cout << "\t" << "spline:\t" << splineResult << std::endl;
	return linearResult && smoothstepResult && lowpassResult && splineResult && cosineResult;
}

const bool test::InterpolationTest::linear () const
{
	const size_t nSteps = 10;
	std::vector<double> x { 0.0, 1.0, 2.0 }, s((x.size() - 1) * nSteps);
	lk::interpolation::linear(begin(x), end(x), begin(s), nSteps);

	bool success = s[0] == 0.0;
	success &= s[1] == 0.1;
	success &= s[11] == 1.1;
	success &= s[19] == 1.9;

	return success;
}

const bool test::InterpolationTest::smoothstep () const
{
	const size_t nSteps = 10;
	std::vector<double> inputVec { 0.0, 3.0, 4.0, 2.0 };
	std::vector<double> outputVec ((inputVec.size() - 1) * nSteps);

	lk::interpolation::smooth(begin(inputVec), end(inputVec), begin(outputVec), nSteps);

	bool success = outputVec[0] == 0.0;
	success &= outputVec[2] == 3.0 * 0.2 * 0.2 * 2.6;
	success &= outputVec[4] == 3.0 * 0.4 * 0.4 * 2.2;
	success &= outputVec[6] == 1.944;
	success &= outputVec[9] == 2.916;
	success &= outputVec[10] == 3.0;
	success &= outputVec[20] == 4.0;

	assert(success);

	return success;
}

const bool test::InterpolationTest::lowpass () const
{
	const size_t nSteps = 10;
	const double filterFactor = 0.5;
	std::vector<double> outputVec(nSteps);
	lk::interpolation::lowpass(begin(outputVec), 0.0, 2.0, nSteps, filterFactor);

	bool success = outputVec[0] == 0.0;
	success &= outputVec[1] == 1.0;
	success &= outputVec[2] == 1.5;
	success &= outputVec[3] == 1.75;
	success &= outputVec[4] == 1.875;
	success &= outputVec[5] == 1.9375;
	success &= outputVec[6] == 1.96875;
	success &= outputVec[7] == 1.984375;
	success &= outputVec[8] == 1.9921875;
	success &= outputVec[9] == 1.99609375;

	std::vector<double> x { 0.0,2.0,2.0,2.0,2.0,2.0,2.0,2.0,2.0, 2.0, };
	std::vector<double> y(x.size());

	lk::interpolation::lowpass(begin(x), end(x), begin(y), nSteps, filterFactor);

	success &= y[0] == 0.0;
	success &= y[1] == 1.0;
	success &= y[2] == 1.5;
	success &= y[3] == 1.75;
	success &= y[4] == 1.875;
	success &= y[5] == 1.9375;
	success &= y[6] == 1.96875;
	success &= y[7] == 1.984375;
	success &= y[8] == 1.9921875;
	success &= y[9] == 1.99609375;

	return success;
}

const bool test::InterpolationTest::cosine () const
{
	const size_t nSteps = 10;

	std::vector<double> x { 0.0, 1.0, 2.0 }, y((x.size() - 1) * nSteps);
	lk::interpolation::cosine(begin(x), end(x), begin(y), nSteps);

	bool success = y[0] == x[0];
	success &= y[10] == x[1];

	return success;
}

const bool test::InterpolationTest::spline () const
{
	const size_t nSteps = 10;
	std::vector<double> inputVec { 0.0, 0.5, 1.5, 2.0, 1.5, 2.0, 3.0, 1.0 };
	std::vector<double> outputVec(nSteps * (inputVec.size() - 1));
	lk::interpolation::spline(begin(inputVec), end(inputVec), begin(outputVec), nSteps);

	bool success = outputVec[0] == inputVec[0];
	success &= outputVec[10] == inputVec[1];
	success &= outputVec[20] == inputVec[2];
	success &= outputVec[30] == inputVec[3];
	success &= outputVec[40] == inputVec[4];
	success &= outputVec[50] == inputVec[5];
	success &= outputVec[60] == inputVec[6];

	return success;
}
