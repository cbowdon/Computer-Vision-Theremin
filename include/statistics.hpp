#ifndef STATISTICS_H
#define STATISTICS_H

#include <numeric>

namespace statistics
{
	// statistics functions for STL iterators
	template <class Iterator, class Function>
		const double average (Iterator it, Iterator end, const Function selector)
		{
			double sum = 0;

			size_t count = 0;
			for (; it != end; ++it)
			{
				sum += selector(*it);
				++count;
			}
			return count == 0 ? sum : sum / count;
		}

	template <class Iterator, class Function>
		const double chiSquared (Iterator itE, Iterator endE, Iterator itO, const Function selector)
		{
			double sum = 0;

			for (; itE != endE; ++itE, itO++)
			{
				double e = selector(*itE);
				double d = selector(*itO) - e;
				sum += d * d / e;
			}
			return sum;
		}

	template <class Iterator, class Function>
		const double variance (Iterator it, Iterator end, const Function selector)
		{
			double avg = average(it, end, selector);

			size_t count = 0;
			double sum = 0;

			for (; it != end; ++it)
			{
				double d = selector(*it) - avg;
				sum += d * d;
				++count;
			}
			return count == 0 ? sum : sum / count;
		}
	

	// default selectors are x -> x
	template <class Iterator>
		const double average (Iterator it, Iterator end)
		{
			return average(it, end, [](double val) { return val; });
		}

	template <class Iterator>
		const double variance (Iterator it, Iterator end)
		{
			return variance(it, end, [](double v) { return v; });
		}

	template <class Iterator>
		const double chiSquared (Iterator itE, Iterator endE, Iterator itO)
		{
			return chiSquared(itE, endE, itO, [](double v) { return v; });
		}


}
#endif
