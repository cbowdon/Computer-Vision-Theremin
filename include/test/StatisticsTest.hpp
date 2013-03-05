#ifndef STATISTICSTEST_H
#define STATISTICSTEST_H

#include <string>
#include <iostream>
#include <complex>
#include "TestBase.hpp"
#include "statistics.hpp"

namespace test
{
	class StatisticsTest : public TestBase
	{
		public:
			bool run () override;
			const std::string getName () const override;

		private:
			const bool average () const;
			const bool variance () const;
			const bool chiSquared () const;
	};
}
#endif 
