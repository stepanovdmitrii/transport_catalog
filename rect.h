#pragma once

#include "shape.h"
#include "point.h"
#include "i_object.h"

#include <iostream>

namespace Svg {
	class Rectangle : public Shape<Rectangle>, public IObject
	{

	private:
		Point _left_upper;
		double _width;
		double _height;
	public:
		Rectangle();
		Svg::Rectangle& SetLeftUpper(Point left_upper);
		Svg::Rectangle& SetWidht(double weight);
		Svg::Rectangle& SetHeight(double height);

		virtual void Render(std::ostream&) override;
	};
}

