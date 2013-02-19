#ifndef LINEARCONVERTERTEST_H
#define LINEARCONVERTERTEST_H

#include <string>
#include <cassert>
#include <iostream>
#include "TestBase.hpp"
#include "LinearConverter.hpp"

namespace test
{
	class LinearConverterTest : public TestBase
	{
		public:
			bool run () override;
			const std::string getName () const override;
		private:
			const bool positiveCalcs () const;
			const bool negativeCalcs () const;
			const bool crossOriginCalcs () const;
			const bool edgeCaseCalcs () const;
	};
}

#endif
