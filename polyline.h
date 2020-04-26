#pragma once

#include "shape.h"
#include "point.h"
#include "i_object.h"

#include <vector>
#include <iostream>

namespace Svg {

	class Polyline : public Shape<Polyline>, public IObject
	{
	public:
		Polyline();

		Polyline& AddPoint(Point);
		
		virtual void Render(std::ostream&) override;
	private:
		std::vector<Point> _points;
	};
}

