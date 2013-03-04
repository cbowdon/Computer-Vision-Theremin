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
	lk::TrackFile trackFile("resources/sample_file_test.txt"); // file not used
	lk::trackDatum datum(trackFile.extractDatum(goodString));

	bool success = datum == lk::trackDatum(1, cv::Point2d(123.456, 678.90));

	const std::string goodString2 = "1, [123, 678]";
	lk::trackDatum datum2(trackFile.extractDatum(goodString2));

	success &= datum2 == lk::trackDatum(1, cv::Point2d(123.0, 678.0));

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
	const std::string filename = "resources/sample_file_test.txt";

	std::vector<lk::trackDatum> inData(20);
	std::fill(begin(inData), end(inData), lk::trackDatum(1, cv::Point2d(1.0, 1.0)));

	lk::TrackFile writeFile(filename);
	writeFile.save(inData);	

	lk::TrackFile readFile(filename);

	std::vector<lk::trackDatum> outData;
	readFile.load(outData);

	bool success = outData.size() == inData.size();

	for (size_t i = 0; i < outData.size(); i++)
	{
		success &= inData[i] == outData[i];
	}

	return success;
}
