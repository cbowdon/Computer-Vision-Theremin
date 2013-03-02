#include "TrackFile.hpp"

lk::TrackFile::TrackFile (std::string filename) :
	filename(filename),
	file(filename, std::fstream::app)
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
		// prevent exception leaving function
	}
}

std::ostream& lk::operator<<(std::ostream& ostr, const lk::trackDatum d)
{
	ostr << std::get<0>(d) << ", " << std::get<1>(d);
	return ostr;
}
