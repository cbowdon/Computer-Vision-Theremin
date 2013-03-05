#include "StatisticsTest.hpp"

const std::string test::StatisticsTest::getName () const
{
	return "Statistics Test";
}

bool test::StatisticsTest::run ()
{
	bool averageResult = average();
	std::cout << "\t" << "average:\t" << averageResult << std::endl;

	bool chiSquaredResult = chiSquared();
	std::cout << "\t" << "chiSquared:\t" << chiSquaredResult << std::endl;

	bool varianceResult = variance();
	std::cout << "\t" << "variance:\t" << varianceResult << std::endl;

	return averageResult && chiSquaredResult && varianceResult;
}

const bool test::StatisticsTest::average () const
{
	std::vector<double> a { 0.1, 0.2, 0.3, 0.4, -0.5, 0.0, 0.2 };

	const double avg = statistics::average(begin(a), end(a));

	bool success = std::abs(avg - 0.1) < 1e-9;

	return success;
}

const bool test::StatisticsTest::chiSquared () const
{
	std::vector<double> a { 0.1, 0.2, 0.3, 0.4, -0.5, 0.0, 0.2 };

	return false;
}

const bool test::StatisticsTest::variance () const
{
	std::vector<double> a { 0.1, 0.2, 0.3, 0.4, -0.5, 0.0, 0.2 };

	const double var = statistics::variance(begin(a), end(a));

	std::cout << var << std::endl;

	bool success = std::abs(var - 0.07428571428571429) < 1e-9;

	return success;
}
