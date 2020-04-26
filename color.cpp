#include "color.h"

Svg::Color::Color(): _value("none")
{
}

Svg::Color::Color(const std::string& value): _value(value)
{
}

Svg::Color::Color(const Rgb& rgb): _value(rgb.ToSvg())
{
}

Svg::Color::Color(const Rgba& rgba) : _value(rgba.ToSvg())
{
}

Svg::Color::Color(const char* value): _value(value)
{
}

std::string Svg::Color::ToSvg() const
{
	return _value;
}
