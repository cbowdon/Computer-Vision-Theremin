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
	std::vector<double> outputVec(nSteps);
	lk::interpolation::linear(begin(outputVec), 0.0, 5.0, nSteps);

	bool success = outputVec[0] == 0.0;
	success &= outputVec[1] == 0.5;
	success &= outputVec[2] == 1.0;
	success &= outputVec[3] == 1.5;
	success &= outputVec[4] == 2.0;
	success &= outputVec[5] == 2.5;
	success &= outputVec[6] == 3.0;
	success &= outputVec[7] == 3.5;
	success &= outputVec[8] == 4.0;
	success &= outputVec[9] == 4.5;

	std::vector<double> x { 0.0, 1.0, 2.0 }, s((x.size() - 1) * nSteps);
	lk::interpolation::s::linear(begin(x), end(x), begin(s), nSteps);

	success &= s[0] == 0.0;
	success &= s[1] == 0.1;
	success &= s[11] == 1.1;
	success &= s[19] == 1.9;

//	std::for_each(begin(s), end(s), [](double d) { std::cout << d << ", "; });
//	std::cout << std::endl;

	return success;
}

const bool test::InterpolationTest::smoothstep () const
{
	const size_t nSteps = 10;
	std::vector<long double> outputVec(nSteps);
	lk::interpolation::smoothstep(begin(outputVec), 0.0, 3.0, nSteps);

	bool success = outputVec[0] == 0.0;
	success &= outputVec[2] == 3.0 * 0.2 * 0.2 * 2.6;
	success &= outputVec[4] == 3.0 * 0.4 * 0.4 * 2.2;
	success &= outputVec[6] == 1.944;
	success &= outputVec[9] == 2.916;
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
	success &= outputVec[1] == 1;
	success &= outputVec[2] == 1.5;
	success &= outputVec[3] == 1.75;
	success &= outputVec[4] == 1.875;
	success &= outputVec[5] == 1.9375;
	success &= outputVec[6] == 1.96875;
	success &= outputVec[7] == 1.984375;
	success &= outputVec[8] == 1.9921875;
	success &= outputVec[9] == 1.99609375;

	return success;
}

const bool test::InterpolationTest::cosine () const
{
	const size_t nSteps = 10;
	std::vector<double> outputVec(nSteps);
	lk::interpolation::cosine(begin(outputVec), 0.0, 2.0, nSteps);

	bool success = outputVec[0] = 0.0;
	success &= outputVec[5] == 1.0;

	std::vector<double> x { 0.0, 1.0, 2.0 }, y((x.size() - 1) * nSteps);
	lk::interpolation::s::cosine(begin(x), end(x), begin(y), nSteps);

	std::for_each(begin(y), end(y), [](double d) { std::cout << d << " "; });
	std::cout << std::endl;

	return success;
}

const bool test::InterpolationTest::spline () const
{
	const size_t nSteps = 10;
	std::vector<double> shortOutputVec(nSteps);
	lk::interpolation::spline(begin(shortOutputVec), 0.0, 0.0, 1.0, 1.0, nSteps);

	std::cout << std::endl;
	bool success = shortOutputVec[0] == 0.0;
	success &= shortOutputVec[5] == 0.5;

	std::vector<double> inputVec { 0.0, 0.5, 1.5, 2.0, 2.0 }; 
	std::vector<double> outputVec(nSteps * inputVec.size());
	lk::interpolation::spline(begin(inputVec), end(inputVec), begin(outputVec), nSteps);

	success &= outputVec[0] == inputVec[0];
	success &= outputVec[10] == inputVec[1];
	success &= outputVec[20] == inputVec[2];
	success &= outputVec[30] == inputVec[3];
	success &= outputVec[40] == inputVec[4];

	std::for_each(begin(outputVec), end(outputVec), [](double d){ std::cout << d << std::endl; });

	return success;
}
