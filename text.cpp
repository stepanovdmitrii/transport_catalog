#include "text.h"

Svg::Text::Text() : _point({ 0,0 }), _offset({ 0,0 }), _font_size(1), _font_family(std::nullopt), _data("")
{
}

Svg::Text& Svg::Text::SetPoint(Point value)
{
	_point = value;
	return *this;
}

Svg::Text& Svg::Text::SetOffset(Point value)
{
	_offset = value;
	return *this;
}

Svg::Text& Svg::Text::SetFontSize(uint32_t value)
{
	_font_size = value;
	return *this;
}

Svg::Text& Svg::Text::SetFontFamily(const std::string& value)
{
	_font_family = value;
	return *this;
}

Svg::Text& Svg::Text::SetData(const std::string& value)
{
	_data = value;
	return *this;
}

Svg::Text& Svg::Text::SetFontWeight(const std::string& value)
{
	_font_weight = value;
	return *this;
}

void Svg::Text::Render(std::ostream& output)
{
	output << "<text x=\"" << _point.x << "\" y=\"" << _point.y << "\" dx=\"" << _offset.x << "\" dy=\"" << _offset.y << "\" font-size=\"" << _font_size << "\" ";
	if (_font_family.has_value()) {
		output << "font-family=\"" << *_font_family << "\" ";
	}
	if (_font_weight.has_value()) {
		output << "font-weight=\"" << *_font_weight << "\" ";
	}
	Svg::Shape<Svg::Text>::RenderCommon(output);
	output << ">" << _data << "</text>";
}


