#ifndef POINTACCOUNTANTTEST_H
#define POINTACCOUNTANTTEST_H

#include <iostream>
#include "TestBase.hpp"
#include "PointAccountant.hpp"

namespace test
{
	class PointAccountantTest : public TestBase
	{
		public:
			bool run () override;
			const std::string getName () const override;
		private:
			bool spawnPoints ();
			bool weedPoints ();
			bool getCenter ();
	};
}

extern std::ostream& operator<<(std::ostream& ostr, const test::PointAccountantTest pat);
#endif
