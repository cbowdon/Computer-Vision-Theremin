#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <vector>
#include <iostream>

namespace lk
{
	namespace interpolation
	{
		template <class Iterator, class T, class Function>
			void interpolate (Iterator it, const T value0, const T value1, size_t nSteps, Function filter)
			{
				std::cout << "params: " << value0 << ", " << value1 << ", " <<  nSteps << std::endl;
				for (double i = 0; i < nSteps; i++, it++)
				{
					T inc = filter(i / nSteps);
					*it = (value1 * inc) + (value0 * (1 - inc));
					std::cout << "i, inc, *it: " << i << ", " << inc << ", " << *it << std::endl;
				}
			}

		template <class Iterator, class T>
			void linear (Iterator it, const T value0, const T value1, size_t nSteps)
			{
				interpolate(it, value0, value1, nSteps, [](T v) { return v; });
			}

		template <class Iterator, class T>
			void smoothstep (Iterator it, const T value0, const T value1, size_t nSteps)
			{
				interpolate(it, value0, value1, nSteps, [](T v) { return v * v * (3 - 2 * v); });
			}

		template <class Iterator, class T>
			void weighted (Iterator it, const T value0, const T value1, size_t nSteps, size_t factor=10)
			{
				interpolate
					(it, value0, value1, nSteps, [value1,factor](T v)
					 { 
					 return (v * (factor - 1) + value1) / factor;
					 }
					);
			}

		template <class Iterator, class T>
			void quadratic (Iterator it, const T value0, const T value1, size_t nSteps);

		template <class Iterator, class T>
			void cubic (Iterator it, const T value0, const T value1, size_t nSteps);

		template <class Iterator, class T>
			void sin (Iterator it, const T value0, const T value1, size_t nSteps);

		template <class Iterator, class T>
			void spline (Iterator it, const T value0, const T value1, size_t nSteps);
	}
}

#endif
