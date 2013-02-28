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

		namespace details
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

			template <class Iterator, class Function>
				void interpolate4 (Iterator it, Iterator end, Iterator out, size_t nSteps, Function func)
				{
					// edge case: first point
					Iterator pm1 = it;
					Iterator p0 = it;
					Iterator p1 = (it+1);
					Iterator p2 = (it+2);

					for (double i = 0; i < nSteps; i++, out++)
					{
						double mu = i / nSteps;
						*out = func(*pm1, *p0, *p1, *p2, mu);
					}

					p0++;
					p1++;
					p2++;

					// middle points
					for (; p2 != end; pm1++, p0++, p1++, p2++)
					{
						for (double i = 0; i < nSteps; i++, out++)
						{
							double mu = i / nSteps;
							*out = func(*pm1, *p0, *p1, *p2, mu);
						}
					}

					// edge case: last point
					p2--;

					for (double i = 0; i < nSteps; i++, out++)
					{
						double mu = i / nSteps;
						*out = func(*pm1, *p0, *p1, *p2, mu);
					}
				}

			template <class T>
				T linearStep (T y0, T y1, T mu)
				{
					return mu * y1 + (1 - mu) * y0;
				}

			template <class T>
				T smoothStep (T y0, T y1, T mu)
				{
					T a = mu * mu * (3 - 2 * mu);
					return a * y1  + (1 - a) * y0;
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
		}

		template <class Iterator>
			void linear (Iterator it, Iterator end, Iterator out, size_t nSteps)
			{
				details::interpolate2(it, end, out, nSteps, details::linearStep<double>);
			}

		template <class Iterator>
			void smooth (Iterator it, Iterator end, Iterator out, size_t nSteps)
			{
				details::interpolate2(it, end, out, nSteps, details::smoothStep<double>);
			}

		template <class Iterator>
			void cosine (Iterator it, Iterator end, Iterator out, size_t nSteps)
			{
				details::interpolate2(it, end, out, nSteps, details::cosineStep<double>);
			}

		template <class Iterator>
			void cubic (Iterator it, Iterator end, Iterator out, size_t nSteps)
			{
				details::interpolate4(it, end, out, nSteps, details::cubicStep<double>);
			}

		template <class Iterator>
			void spline (Iterator it, Iterator end, Iterator out, size_t nSteps)
			{
				details::interpolate4(it, end, out, nSteps, details::splineStep<double>);
			}

		template <class Iterator, class T>
			void lowpass (Iterator it, const T value0, const T value1, size_t nSteps, double filterFactor=0.5)
			{
				//				 s[0] = x[0]
				*it = value0;
				T prev = *it;
				it++;

				//				 s[i] = a * x[i] + (1-a) * s[i-1]
				for (double i = 1; i < nSteps; i++, it++)
				{
					*it = (value1 * filterFactor) + (prev * (1.0 - filterFactor));
					prev = *it;
				}
			}

		template <class Iterator>
			void lowpass (Iterator it, Iterator end, Iterator out, double filterFactor)
			{
				*out = *it;
				out++;
				it++;

				double prev = *out;
				for (; it != end; it++, out++)
				{
					*out = (*it * filterFactor) + (prev * (1.0 - filterFactor));
					prev = *out;
				}
			}

		template <class Iterator>
			void lowpass (Iterator it, Iterator end, Iterator out, size_t nSteps, double filterFactor)
			{
				*out = *it;
				out++;
				it++;

				for (; it != end; it++)
				{
					for (size_t i = 0; i < nSteps; i++)
					{
						*out = *it;						
						out++;
					}
				}
				//				size_t j = 0;
				//				*out = *it;
				//				out++;
				//				it++;
				//				std::cout << "s[" << j << "] = " << *out << std::endl;
				//				j++;
				//				double prev = *out;

				//				for (double i = 0; i < nSteps; i++, out++)
				//				{
				//					*out = (*it * filterFactor) + (prev * (1.0 - filterFactor));
				//					prev = *out;
				//					std::cout << "s[" << j << "] = " << *out << std::endl;
				//					j++;
				//				}

				//				for (; (it+2) != end; it++)
				//				{
				//					for (double i = 0; i < nSteps; i++, out++)
				//					{
				//						*out = (*it * filterFactor) + (prev * (1.0 - filterFactor));
				//						prev = *out;
				//						std::cout << "s[" << j << "] = " << *out << std::endl;
				//						j++;
				//					}
				//				}
			}
	}
}

#endif
