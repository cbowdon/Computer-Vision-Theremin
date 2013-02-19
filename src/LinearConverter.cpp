#include "LinearConverter.hpp"

lk::LinearConverter::LinearConverter () :
	xMin(0.0),
	xMax(1.0),
	xDist(1.0),
	yMin(0.0),
	yMax(1.0),
	yDist(1.0)
{
	calcLine();
}

void lk::LinearConverter::setXRange (float min, float max)
{
	if (min > max)
	{
		std::swap(min, max);
	}
	xMin = min;
	xMax = max;
	xDist = xMax - xMin;
	calcLine();
}

void lk::LinearConverter::setYRange (float min, float max)
{
	if (min > max)
	{
		std::swap(min, max);
	}
	yMin = min;
	yMax = max;
	yDist = yMax - yMin;
	calcLine();
}

const float lk::LinearConverter::convertXToY (float xValue) const
{
	return gradient * xValue + intercept;
}

const float lk::LinearConverter::convertYToX (float yValue) const
{
	return (yValue - intercept) / gradient;
}

void lk::LinearConverter::calcLine ()
{
	gradient = yDist / xDist;
	intercept = yMin - gradient * xMin;
}
