#ifndef TRACKFILETEST_H
#define TRACKFILETEST_H

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "TestBase.hpp"
#include "TrackFile.hpp"

namespace test
{
	class TrackFileTest : public TestBase
	{
		public:
			bool run () override;
			const std::string getName () const override;
		private:
			const bool extractDatum () const;
			const bool loadData () const;

	};
}

#endif
