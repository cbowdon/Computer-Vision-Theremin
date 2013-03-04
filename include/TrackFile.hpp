#ifndef TRACKFILE_H
#define TRACKFILE_H

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <regex>
#include <stdexcept>
#include <sstream>
#include <opencv2/core/core.hpp>

namespace lk
{
	typedef std::pair<size_t, cv::Point2d> trackDatum;

	extern std::ostream& operator<<(std::ostream& ostr, const lk::trackDatum d);

	class TrackFile
	{

		public:

			TrackFile (const std::string filename);
			~TrackFile ();

			TrackFile () = delete;
			TrackFile (const TrackFile& copy) = delete;
			TrackFile& operator=(const TrackFile& rhs) = delete;

			template <class Container>
				void load (Container& data)
				{
					file = std::fstream(filename, std::fstream::in);

					std::vector<std::string> strData;

					std::string temp;
					while (std::getline(file, temp))
					{
						strData.push_back(temp);
					}

					data.resize(strData.size());

					std::transform(begin(strData), end(strData), begin(data), 
							[this](const std::string str) { return extractDatum(str); });

					file.close();
				}

			template <class Container>
				void save (const Container& data)
				{
					file = std::fstream(filename, std::fstream::out);
					std::for_each(begin(data), end(data),[this](trackDatum d) { file << d << std::endl; });
					file.close();
				}

			const trackDatum extractDatum (std::string str);

		private:

			const std::string filename;
			std::fstream file;
	};
}

#endif
