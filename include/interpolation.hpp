#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <vector>
#include <iostream>
#include <functional>

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
			void lowpass (Iterator it, const T value0, const T value1, size_t nSteps, size_t factor=10)
			{
				std::function<T(T)> lp = [value1,factor](T v) { return (v * (factor - 1) + value1) / factor; };
				interpolate(it, value0, value1, nSteps, lp);
			}

		template <class Iterator, class T>
			void spline (Iterator it, const T p0, const T p1, const T p2, const T p3, size_t nSteps)
			{
				std::function<T(T)> catmullRom =
					[p0, p1, p2, p3](T v)
					{
						return 0.5 * (
								(2 * p1) + 
								(-p0 + p2) * v +
								(2 * p0 - 5 * p1 + 4 * p2 - p3) * v * v +
								(-p0 + 3 * p1 - 3 * p2 + p3) * v * v * v);

					};
				interpolate(it, p1, p2, nSteps, catmullRom);
			}

		template <class Iterator>
			void spline (Iterator it, Iterator end, Iterator out, size_t nSteps)
			{

				std::cout << "spline" << std::endl;
				Iterator p0 = it, p1 = it, p2 = it, p3 = it;

				// manually do first
				p2 += 1;
				p3 += 2;
				std::cout << 0  << ": " << *p0 << ", " << *p1 << ", " << *p2 << ", " << *p3 << std::endl;
				spline(out, *p0, *p1, *p2, *p3, nSteps);
				std::cout << std::endl;

				// prepare for loop
				p1++;
				p2++;
				p3++;

				for (; p3 != end; p0++, p1++, p2++, p3++)
				{
					std::cout << "n: " << *p0 << ", " << *p1 << ", " << *p2 << ", " << *p3 << std::endl;
					spline(out, *p0, *p1, *p2, *p3, nSteps);				
					std::cout << std::endl;
				}

				// manually do last
				p0++;
				p1++;
				p2++;
				std::cout << "N: " << *p0 << ", " << *p1 << ", " << *p2 << ", " << *p3 << std::endl;
				spline(out, *p0, *p1, *p2, *p3, nSteps);
				std::cout << std::endl;
			}

		template <class Iterator, class T>
			void quadratic (Iterator it, const T value0, const T value1, size_t nSteps);

		template <class Iterator, class T>
			void cubic (Iterator it, const T value0, const T value1, size_t nSteps);

		template <class Iterator, class T>
			void sin (Iterator it, const T value0, const T value1, size_t nSteps);

	}
}

#endif
