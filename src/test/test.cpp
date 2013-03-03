#include <algorithm>
#include <iostream>
#include "PointAccountantTest.hpp"
#include "LinearConverterTest.hpp"
#include "NoteGeneratorTest.hpp"
#include "NoteTest.hpp"
#include "InterpolationTest.hpp"
#include "TrackFileTest.hpp"

using namespace test;

int main (int argc, char** argv)
{
	std::vector< std::shared_ptr<TestBase> > tests;
	tests.push_back(std::shared_ptr<TestBase>(new PointAccountantTest));
	tests.push_back(std::shared_ptr<TestBase>(new LinearConverterTest));
	tests.push_back(std::shared_ptr<TestBase>(new NoteGeneratorTest));
	tests.push_back(std::shared_ptr<TestBase>(new NoteTest));
	tests.push_back(std::shared_ptr<TestBase>(new InterpolationTest));
	tests.push_back(std::shared_ptr<TestBase>(new TrackFileTest));

	for (auto& test : tests)
	{
		std::cout << test->getName() << "... " << std::endl;
		bool result = test->run();
		std::cout << (result ? "Ok" : "Not Ok") << std::endl;
	}

	return EXIT_SUCCESS;
}
