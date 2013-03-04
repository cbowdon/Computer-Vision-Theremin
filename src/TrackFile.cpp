#include "TrackFile.hpp"

lk::TrackFile::TrackFile (const std::string filename) :
	filename(filename),
	file(filename)
{

}

lk::TrackFile::~TrackFile ()
{
	try
	{
		if (file.is_open())
		{
			file.close();
		}
	}
	catch (...)
	{
		// prevent all exceptions leaving dtor
	}
}

std::ostream& lk::operator<<(std::ostream& ostr, const lk::trackDatum d)
{
	ostr << std::get<0>(d) << ", " << std::setiosflags(std::ios::fixed) << std::setprecision(2) << std::get<1>(d);
	return ostr;
}

const lk::trackDatum lk::TrackFile::extractDatum (const std::string str)
{
	std::regex regex(R"((\d+), \[(\d+.?\d*), (\d+.?\d*)\])");
	std::smatch match;
	std::regex_match(str, match, regex);

	size_t frameCount = 0;
	double x = 0;
	double y = 0;
	const std::string couldNotParse("Could not parse text data");

	if (match.size() == 4)
	{
		std::istringstream iss1(match[1]);
		std::istringstream iss2(match[2]);
		std::istringstream iss3(match[3]);

		if (iss1 >> frameCount && iss2 >> x && iss3 >> y)
		{
			return lk::trackDatum(frameCount, cv::Point2d(x, y));
		}
	}
	std::cout << couldNotParse << " " << str << std::endl;
	throw std::invalid_argument(couldNotParse);
}
