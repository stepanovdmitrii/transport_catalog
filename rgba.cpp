#include "rgba.h"

Svg::Rgba::Rgba() : red(0), green(0), blue(0), alpha(0.0)
{
}

Svg::Rgba::Rgba(std::uint8_t red_value, std::uint8_t green_value, std::uint8_t blue_value, double alpha_value): red(red_value), green(green_value), blue(blue_value), alpha(alpha_value)
{
}

std::string Svg::Rgba::ToSvg() const
{
	std::stringstream ss;
	ss << "rgba(" << (size_t)red << "," << (size_t)green << "," << (size_t)blue << "," << alpha << ")";
	return ss.str();
}
