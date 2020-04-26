#include "polyline.h"

Svg::Polyline::Polyline()
{
}

Svg::Polyline& Svg::Polyline::AddPoint(Svg::Point value)
{
	_points.push_back(value);
	return *this;
}

void Svg::Polyline::Render(std::ostream& output)
{
	output << "<polyline points=\"";
	for (const auto& p : _points) {
		output << p.x << "," << p.y << " ";
	}
	output << "\" ";
	Svg::Shape<Svg::Polyline>::RenderCommon(output);
	output << "/>";
}
