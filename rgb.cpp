#include "rgb.h"

Svg::Rgb::Rgb() : red(0), green(0), blue(0)
{
}

Svg::Rgb::Rgb(std::uint8_t red_value, std::uint8_t green_value, std::uint8_t blue_value): red(red_value), green(green_value), blue(blue_value)
{
}

std::string Svg::Rgb::ToSvg() const
{
	std::stringstream ss;
	ss << "rgb(" << (size_t)red << "," << (size_t)green << "," << (size_t)blue << ")";
	return ss.str();
}
