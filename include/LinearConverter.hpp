#ifndef LINEARCONVERTER_H
#define LINEARCONVERTER_H

#include <algorithm>

namespace lk
{
	class LinearConverter
	{
		public:
			LinearConverter();
			LinearConverter(LinearConverter& lc) = delete;
			LinearConverter operator=(LinearConverter& lc) = delete;

			const float convertXToY (float xValue) const;
			const float convertYToX (float xValue) const;
			void setXRange (float min, float max);
			void setYRange (float min, float max);

		private:

			float xMin, xMax, xDist;
			float yMin, yMax, yDist;
			float gradient, intercept;

			void calcLine();
	};
}

#endif
