#include "TrackFileTest.hpp"

const std::string test::TrackFileTest::getName () const
{
	return "TrackFile Test";
}

bool test::TrackFileTest::run ()
{
	bool extractDatumResult = extractDatum();
	bool loadDataResult = loadData();
	std::cout << "\t" << "extractDatum:\t" << extractDatumResult << std::endl;
	std::cout << "\t" << "loadData:\t" << loadDataResult << std::endl;
	return extractDatumResult && loadDataResult;
}

const bool test::TrackFileTest::extractDatum () const
{
	const std::string goodString = "1, [123.456, 678.90]";
	lk::TrackFile trackFile("resources/test.txt");
	lk::trackDatum datum(trackFile.extractDatum(goodString));

	bool success = datum == lk::trackDatum(1, cv::Point2d(123.456, 678.90));

	bool didThrow = false;
	try
	{
		const std::string badString = "1, [-415.0, 152.0]";
		trackFile.extractDatum(badString);
	}
	catch (std::invalid_argument& e)
	{
		didThrow = true;
	}

	success &= didThrow;

	return success;
}

const bool test::TrackFileTest::loadData () const
{
	std::vector<lk::trackDatum> inData(20);
	std::fill(begin(inData), end(inData), lk::trackDatum(1, cv::Point2d(1.0, 1.0)));

	lk::TrackFile trackFile("resources/test.txt");
	trackFile.save(inData);	

	std::vector<lk::trackDatum> outData;
	trackFile.load(outData);

	bool success = outData.size() == inData.size();

	for (size_t i = 0; i < outData.size(); i++)
	{
		std::cout << std::get<0>(inData[i]) << ", " << std::get<0>(outData[i]) << std::endl;
		std::cout << std::get<1>(inData[i]) << ", " << std::get<1>(outData[i]) << std::endl;
		success &= inData[i] == outData[i];
	}

	return success;
}
