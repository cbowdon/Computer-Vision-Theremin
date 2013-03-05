#ifndef STATISTICS_H
#define STATISTICS_H

#include <numeric>

namespace statistics
{
	namespace details
	{
		template <class Iterator, class Function>
			const typename Iterator::value_type average (Iterator it, Iterator end, const Function selector)
			{
				typedef typename Iterator::value_type valueType;

				valueType sum = valueType();

				size_t count = 0;
				for (; it != end; ++it)
				{
					sum += selector(*it);
					++count;
				}
				return count == 0 ? 0 : sum / count;
			}

		template <class Container, class Function>
			const double chiSquared (const Container& expected, const Container& actual, const Function selector)
			{
				return 0;
			}

		template <class Iterator, class Function>
			const typename Iterator::value_type variance (Iterator it, Iterator end, const Function selector)
			{
				typedef typename Iterator::value_type valueType;

				valueType avg = average(it, end, selector);
				
				size_t count = 0;
				valueType sum = valueType();

				for (; it != end; ++it)
				{
					valueType d = selector(*it) - avg;
					sum += d * d;
					++count;
				}
				return count == 0 ? 0 : sum / count;
			}

	}

	template <class Iterator>
		const typename Iterator::value_type average (Iterator it, Iterator end)
		{
			return details::average(it, end, [](typename Iterator::value_type val) { return val; });
		}

	template <class Iterator>
		const typename Iterator::value_type variance (Iterator it, Iterator end)
		{
			return details::variance(it, end, [](typename Iterator::value_type v) { return v; });
		}

	template <class Container>
		const double chiSquared (const Container& expected, const Container& actual)
		{
			return 0;
		}


}
#endif
