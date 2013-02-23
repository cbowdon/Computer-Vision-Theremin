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
	bool splineResult = spline();
	std::cout << "\t" << "linear:\t" << linearResult << std::endl;
	std::cout << "\t" << "smoothstep:\t" << smoothstepResult << std::endl;
	std::cout << "\t" << "lowpass:\t" << lowpassResult << std::endl;
	std::cout << "\t" << "spline:\t" << splineResult << std::endl;
	return linearResult && smoothstepResult && lowpassResult && splineResult;
}

const bool test::InterpolationTest::linear () const
{
	const size_t nSteps = 10;
	std::vector<double> outputVec(nSteps);
	lk::interpolation::linear(begin(outputVec), 0.0, 1.0, nSteps);

	bool success = outputVec[0] == 0.0;
	success &= outputVec[1] == 0.1;
	success &= outputVec[2] == 0.2;
	success &= outputVec[3] == 0.3;
	success &= outputVec[4] == 0.4;
	success &= outputVec[5] == 0.5;
	success &= outputVec[6] == 0.6;
	success &= outputVec[7] == 0.7;
	success &= outputVec[8] == 0.8;
	success &= outputVec[9] == 0.9;

	return success;
}

const bool test::InterpolationTest::smoothstep () const
{
	const size_t nSteps = 10;
	std::vector<double> outputVec(nSteps);
	lk::interpolation::smoothstep(begin(outputVec), 0.0, 1.0, nSteps);

	bool success = outputVec[0] == 0.0;
	success &= outputVec[2] == 0.2 * 0.2 * 2.6;
	success &= outputVec[4] == 0.4 * 0.4 * 2.2;
	success &= outputVec[6] == 0.6 * 0.6 * 1.8;
	success &= outputVec[8] == 0.8 * 0.8 * 1.4;
	success &= outputVec[9] == 0.9 * 0.9 * 1.2;

	return success;
}

const bool test::InterpolationTest::lowpass () const
{
	const size_t nSteps = 10;
	const size_t filterFactor = 5;
	std::vector<double> outputVec(nSteps);
	lk::interpolation::lowpass(begin(outputVec), 0.0, 1.0, nSteps, filterFactor);

	bool success = outputVec[0] == 0.2;
	success &= outputVec[2] == 1.8 / 5;
	success &= outputVec[4] == 2.6 / 5;
	success &= outputVec[6] == 3.4 / 5;
	success &= outputVec[8] == 4.2 / 5;
	success &= outputVec[9] == 4.6 / 5;

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
