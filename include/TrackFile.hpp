#ifndef TRACKFILE_H
#define TRACKFILE_H

#include <string>
#include <iostream>
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
		typedef std::istream_iterator<std::string> istr_it;

		public:

			TrackFile (const std::string filename);
			~TrackFile ();

			TrackFile () = delete;
			TrackFile (const TrackFile& copy) = delete;
			TrackFile& operator=(const TrackFile& rhs) = delete;

			template <class Container>
				void load (Container& data)
				{
					std::vector<std::string> strData;
					std::copy(istr_it(file), istr_it(), std::back_inserter(strData));
					std::cout << "Strings" << std::endl;

					std::for_each(begin(strData), end(strData),[this](const std::string d) { std::cout << d << std::endl; });

					std::cout << "Data" << std::endl;
					std::transform(begin(strData), end(strData), begin(data), 
							[this](const std::string str) { return extractDatum(str); });
					std::for_each(begin(data), end(data),[this](trackDatum d) { std::cout << d << std::endl; });
				}

			template <class Container>
				void save (const Container& data)
				{
					std::for_each(begin(data), end(data),[this](trackDatum d) { file << d << std::endl; });
				}

			const trackDatum extractDatum (std::string str);

		private:

			const std::string filename;
			std::fstream file;
	};
}

#endif
