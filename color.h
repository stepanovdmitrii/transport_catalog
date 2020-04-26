#pragma once

#include "rgb.h"
#include "rgba.h"

#include <string>

namespace Svg {
	class Color
	{
	public:
		Color();
		Color(const std::string&);
		Color(const Rgb&);
		Color(const Rgba&);
		Color(const char*);

		std::string ToSvg() const;
	private:
		std::string _value;
	};

	const Color NoneColor = Color();
}

