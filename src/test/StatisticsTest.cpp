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
	const std::vector<double> a { 0.1, 0.2, 0.3, 0.4, -0.5, 0.0, 0.2 };

	const double avg = statistics::average(begin(a), end(a));

	bool success = std::abs(avg - 0.1) < 1e-9;

	return success;
}

const bool test::StatisticsTest::chiSquared () const
{
//	const std::vector<double> a { 0.1, 0.2, 0.3, 0.4, -0.5, 0.0, 0.2 };
//	const std::vector<double> b { 0.15, 0.23, 0.32, 0.39, -0.44, -0.03, 0.14 };
	const std::vector<double> a { 660.0, 220.0 };
	const std::vector<double> b { 639.0, 241.0 };

	const double chiSquared = statistics::chiSquared(begin(a), end(a), begin(b));

	std::cout << chiSquared << std::endl;

	bool success = std::abs(chiSquared - 2.668) < 1e-9;

	return success;
}

const bool test::StatisticsTest::variance () const
{
	const std::vector<double> a { 0.1, 0.2, 0.3, 0.4, -0.5, 0.0, 0.2 };

	const double var = statistics::variance(begin(a), end(a));

	bool success = std::abs(var - 0.07428571428571429) < 1e-9;

	return success;
}
