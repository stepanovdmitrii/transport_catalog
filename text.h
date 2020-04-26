#pragma once

#include "shape.h"
#include "point.h"
#include "i_object.h"

#include <string>
#include <optional>
#include <iostream>

namespace Svg {
	class Text : public Shape<Text>, public IObject
	{
	public:
		Text();

		Text& SetPoint(Point);
		Text& SetOffset(Point);
		Text& SetFontSize(uint32_t);
		Text& SetFontFamily(const std::string&);
		Text& SetData(const std::string&);
		Text& SetFontWeight(const std::string&);

		virtual void Render(std::ostream&) override;
	private:
		Point _point;
		Point _offset;
		uint32_t _font_size;
		std::optional<std::string> _font_family;
		std::optional<std::string> _font_weight;
		std::string _data;
	};
}

