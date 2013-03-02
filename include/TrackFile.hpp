#ifndef TRACKFILE_H
#define TRACKFILE_H

#include <string>
#include <iostream>
#include <fstream>
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
				std::copy(istr_it(file), istr_it(), std::back_inserter(data));
			}

		template <class Container>
			void save (const Container& data)
			{
				std::for_each(begin(data), end(data),[this](trackDatum d) { std::cout << d << std::endl; });
				std::for_each(begin(data), end(data),[this](trackDatum d) { file << d << std::endl; });
			}

		private:

		const std::string filename;
		std::fstream file;
	};
}

#endif
