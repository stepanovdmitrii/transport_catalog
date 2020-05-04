#include "rect.h"

Svg::Rectangle::Rectangle(): _left_upper(0,0), _width(0), _height(0)
{
}

Svg::Rectangle& Svg::Rectangle::SetLeftUpper(Point left_upper)
{
	_left_upper = left_upper;
	return *this;
}

Svg::Rectangle& Svg::Rectangle::SetWidht(double width)
{
	_width = width;
	return *this;
}

Svg::Rectangle& Svg::Rectangle::SetHeight(double height)
{
	_height = height;
	return *this;
}

void Svg::Rectangle::Render(std::ostream& output)
{
	output << "<rect x=\"" << _left_upper.x << "\" y=\"" << _left_upper.y << "\" width=\"" << _width << "\" height=\"" << _height << "\" ";
	Svg::Shape<Svg::Rectangle>::RenderCommon(output);
	output << "/>";
}
