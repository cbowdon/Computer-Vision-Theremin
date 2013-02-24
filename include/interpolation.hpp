#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <cmath>
#include <vector>
#include <iostream>
#include <functional>

namespace lk
{
	namespace interpolation
	{
		const double PI = 3.14159265;

		namespace s
		{

			template <class Iterator, class Function>
				void interpolate2 (Iterator it, Iterator end, Iterator out, size_t nSteps, Function func)
				{
					double prev = *it;
					it++;
					for (; it != end; it++)
					{
						for (double i = 0; i < nSteps; i++, out++)
						{
							double mu = i / nSteps;
							*out = func(prev, *it, mu);
						}
						prev = *it;
					}
				}

			template <class T>
				T linearStep (T y0, T y1, T mu)
				{
					return mu * y1 + (1 - mu) * y0;
				}

			template <class T>
				T cosineStep (T y0, T y1, T mu)
				{
					T a = (1.0 - cos(mu * PI)) / 2.0;
					return a * y1  + (1 - a) * y0;
				}

			template <class T>
				T cubicStep (T y0, T y1, T y2, T y3, T mu)
				{
					T mu2 = mu * mu;
					T a0 = y3 - y2 - y0 + y1;
					T a1 = y0 - y1 - a0;
					T a2 = y2 - y0;
					T a3 = y1;

					return a0 * mu * mu2 + a1 * mu2 + a2 * mu2 + a3;
				}

			template <class T>
				T splineStep (T y0, T y1, T y2, T y3, T mu)
				{
					T mu2 = mu * mu;
					T a0 = -0.5 * y0 + 1.5 * y1 - 1.5 * y2 + 0.5 * y3;
					T a1 = y0 - 2.5 * y1 + 2.0 * y2 - 0.5 * y3;
					T a2 = -0.5 * y0 + 0.5 * y2;
					T a3 = y1;

					return a0 * mu * mu2 + a1 * mu2 + a2 * mu2 + a3;
				}

			template <class Iterator>
				void linear (Iterator it, Iterator end, Iterator out, size_t nSteps)
				{
					interpolate2(it, end, out, nSteps, linearStep<double>);
				}

			template <class Iterator>
				void cosine (Iterator it, Iterator end, Iterator out, size_t nSteps)
				{
					interpolate2(it, end, out, nSteps, cosineStep<double>);
				}
		}

		template <class Iterator, class T, class Function>
			void interpolate (Iterator it, const T value0, const T value1, size_t nSteps, Function filter)
			{
//				std::cout << "\tparams: " << value0 << ", " << value1 << ", " <<  nSteps << std::endl;
				for (double i = 0.0; i < nSteps; i++, it++)
				{
					T inc = filter(i / nSteps);
					*it = (value1 * inc) + (value0 * (1.0 - inc));
//					std::cout << "\ti, inc, *it: " << i << ", " << inc << ", " << *it << std::endl;
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
			void lowpass (Iterator it, const T value0, const T value1, size_t nSteps, double filterFactor=0.5)
			{
				std::cout << "\tparams: " << value0 << ", " << value1 << ", " <<  nSteps << std::endl;

				// s[0] = x[0]
				*it = value0;
				T prev = *it;
				it++;

				std::cout << "\ts[0] = " << *it << std::endl;

				// s[i] = a * x[i] + (1-a) * s[i-1]
				for (double i = 1; i < nSteps; i++, it++)
				{
					*it = (value1 * filterFactor) + (prev * (1.0 - filterFactor));
					prev = *it;
					std::cout << "\ts[" << i << "] = " << *it << std::endl;
				}
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

		template <class Iterator, class T>
			void cubic (Iterator it, const T p0, const T p1, const T p2, const T p3, size_t nSteps)
			{
				std::function<T(T)> c =
					[p0, p1, p2, p3](T v)
					{
						T v2 = v * v;
						T a0 = p3 - p2 - p0 + p1;
						T a1 = p0 - p1 - a0;
						T a2 = p2 - p0;
						T a3 = p1;
					};
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

				out += nSteps;

				// prepare for loop
				p1++;
				p2++;
				p3++;

				for (; p3 != end; p0++, p1++, p2++, p3++)
				{
					std::cout << "n: " << *p0 << ", " << *p1 << ", " << *p2 << ", " << *p3 << std::endl;
					spline(out, *p0, *p1, *p2, *p3, nSteps);				
					std::cout << std::endl;
					out += nSteps;
				}

				// manually do last
				p0++;
				p1++;
				p2++;
				std::cout << "N: " << *p0 << ", " << *p1 << ", " << *p2 << ", " << *p3 << std::endl;
				spline(out, *p0, *p1, *p2, *p3, nSteps);
				out += nSteps;
				std::cout << std::endl;
			}

		template <class Iterator, class T>
			void cosine (Iterator it, const T value0, const T value1, size_t nSteps)
			{
				interpolate(it, value0, value1, nSteps, [](T v){ return (1.0 - cos(v*PI))/2.0; });
			}

	}
}

#endif
