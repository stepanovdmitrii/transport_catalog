#pragma once

#include "shape.h"
#include "point.h"
#include "i_object.h"

#include <iostream>

namespace Svg {
	class Circle: public Shape<Circle>, public IObject
	{
	public:
		Circle();

		Circle& SetCenter(Point);
		Circle& SetRadius(double);
		virtual void Render(std::ostream&) override;
	private:
		Point _center;
		double _radius;
	};
}

