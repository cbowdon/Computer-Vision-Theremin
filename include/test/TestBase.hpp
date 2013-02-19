#ifndef TESTBASE_H
#define TESTBASE_H

#include <string>
#include <vector>

namespace test
{
	class TestBase
	{
		public:
			virtual bool run () = 0;
			virtual const std::string getName () const = 0;
	};
}
#endif
