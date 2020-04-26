#include "circle.h"

Svg::Circle::Circle() : _center({0.0, 0.0}), _radius(1.0)
{
}

Svg::Circle& Svg::Circle::SetCenter(Point value)
{
	_center = value;
	return *this;
}

Svg::Circle& Svg::Circle::SetRadius(double value)
{
	_radius = value;
	return *this;
}

void Svg::Circle::Render(std::ostream& output)
{
	output << "<circle cx=\"" << _center.x << "\" cy=\"" << _center.y << "\" r=\"" << _radius << "\" ";
	Svg::Shape<Svg::Circle>::RenderCommon(output);
	output << "/>";
}
