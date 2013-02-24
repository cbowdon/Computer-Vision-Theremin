#ifndef INTERPOLATIONTEST_H
#define INTERPOLATIONTEST_H

#include <string>
#include <cassert>
#include <iostream>
#include <algorithm>
#include "TestBase.hpp"
#include "interpolation.hpp"

namespace test
{
	class InterpolationTest : public TestBase
	{
		public:
			bool run () override;
			const std::string getName () const override;
		private:
			const bool linear () const;
			const bool smoothstep () const;
			const bool lowpass () const;
			const bool spline () const;
			const bool cosine () const;
	};
}

#endif
