#include "TrackFile.hpp"

lk::TrackFile::TrackFile (std::string filename) :
	filename(filename),
	file(filename, std::fstream::out)
{

}

lk::TrackFile::~TrackFile ()
{
	try
	{
		file.close();
	}
	catch (...)
	{
		// prevent invalid_argument leaving function
	}
}

std::ostream& lk::operator<<(std::ostream& ostr, const lk::trackDatum d)
{
	ostr << std::get<0>(d) << ", " << std::get<1>(d);
	return ostr;
}

const lk::trackDatum lk::TrackFile::extractDatum (const std::string str)
{
	std::regex regex(R"((\d+), \[(\d+.\d+), (\d+.\d+)\])");
	std::smatch match;
	std::regex_match(str, match, regex);

	size_t frameCount = 0;
	double x = 0;
	double y = 0;
	const std::string couldNotParse("Could not parse text data");

	if (match.size() == 4)
	{
		std::istringstream iss1(match[1]);
		if (!(iss1 >> frameCount))
		{
			throw std::invalid_argument(couldNotParse);
		}

		std::istringstream iss2(match[2]);
		if (!(iss2 >> x))
		{
			throw std::invalid_argument(couldNotParse);
		}

		std::istringstream iss3(match[3]);
		if (!(iss3 >> y))
		{
			throw std::invalid_argument(couldNotParse);
		}

		return lk::trackDatum(frameCount, cv::Point2d(x, y));
	}
	throw std::invalid_argument(couldNotParse);
}
